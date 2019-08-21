#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <string_view>
#include <cpp_essentials/meta/serialization_json.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/morphological_operations.hpp>
#include <cpp_essentials/gx/kernels.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/expected.hpp>
#include <cpp_essentials/graphs/algorithm.hpp>
#include <cpp_essentials/graphs/tree.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/core/optional_helpers.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <cpp_essentials/core/any_range.hpp>

#include <cpp_essentials/kine/animation.hpp>
#include <cpp_essentials/geo/orientation.hpp>
#include <cpp_essentials/geo/clamp.hpp>
#include <cpp_essentials/core/generator.hpp>
#include <cpp_essentials/geo/dcel.hpp>
#include <cpp_essentials/core/program_args.hpp>
#include <cpp_essentials/core/match.hpp>

#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/geo/triangulation.hpp>

using namespace cpp_essentials;

using binary_func_t = std::function<double(double, double)>;
using unary_func_t = std::function<double(double)>;
using func_t = std::function<double(const std::vector<double>&)>;

struct infix_oper_info
{
    core::cstring_view name;
    int precedence;
    binary_func_t op;
};

struct prefix_oper_info
{
    core::cstring_view name;
    unary_func_t op;
};

struct context_t
{
    using vars_t = std::unordered_map<std::string, double>;
    using funcs_t = std::unordered_map<std::string, func_t>;

    vars_t vars;
    funcs_t funcs;
};

using evaluator_t = std::function<double(context_t&)>;

static const std::vector<infix_oper_info> infix_opers
{
    { core::c_str("=="), 2, core::equal_to },
    { core::c_str("!="), 2, core::not_equal_to },
    { core::c_str("<="), 2, core::less_equal },
    { core::c_str(">="), 2, core::greater_equal },
    { core::c_str("&&"), 4, core::logical_and },
    { core::c_str("||"), 4, core::logical_or },

    { core::c_str("="), 1, nullptr },

    { core::c_str("<"), 2, core::less },
    { core::c_str(">"), 2, core::greater },

    { core::c_str("^"), 5, math::pow },
    { core::c_str("*"), 4, core::multiplies },
    { core::c_str("/"), 4, core::divides },
    { core::c_str("+"), 2, core::plus },
    { core::c_str("-"), 2, core::minus },
};

static const std::vector<prefix_oper_info> prefix_opers
{
    { core::c_str("~"), [](double x) -> double { return !(bool)x; } },
    { core::c_str("+"), core::identity },
    { core::c_str("-"), core::negate },
};

using infix_oper_result = std::tuple<core::cstring_view, const infix_oper_info&, core::cstring_view>;

auto find_infix_oper(core::cstring_view text) -> core::optional<infix_oper_result>
{
    int state = 0;
    core::cstring_view cur = text;
    core::cstring_view op_substr{ cur.end(), cur.end() };
    int min_precedence = std::numeric_limits<int>::max();
    const infix_oper_info* oper_info = nullptr;
    while (!cur.empty())
    {
        const char ch = cur.front();
        if (ch == '(')
        {
            ++state;
        }
        else if (ch == ')')
        {
            --state;
        }

        if (state == 0)
        {
            for (const infix_oper_info& info : infix_opers)
            {
                if (core::starts_with(cur, info.name) && info.precedence < min_precedence)
                {
                    min_precedence = info.precedence;
                    op_substr = cur | sq::take(info.name.size());
                    oper_info = &info;
                }
            }
        }

        cur = cur | sq::drop(1);
    }

    if (oper_info)
    {
        return infix_oper_result{
            text | sq::split_before(op_substr.begin()) | sq::trim_while(core::is_space),
            *oper_info,
            text | sq::split_after(op_substr.end()) | sq::trim_while(core::is_space),
        };
    }
    else
    {
        return core::none;
    }
}

using prefix_oper_result = std::tuple<const prefix_oper_info&, core::cstring_view>;

auto find_prefix_oper(core::cstring_view text) -> core::optional<prefix_oper_result>
{
    for (const prefix_oper_info& info : prefix_opers)
    {
        if (core::starts_with(text, info.name))
        {
            if (auto sub = text | sq::drop(info.name.size()); !sub.empty())
            {
                return prefix_oper_result{ info, sub };
            }
        }
    }
    return core::none;
}

bool valid_brackets(core::cstring_view text)
{
    int state = 0;
    for (char ch : text)
    {
        if (ch == '(')
        {
            ++state;
        }
        else if (ch == ')')
        {
            if (--state < 0)
            {
                return false;
            }
        }
    }
    return state == 0;
}

evaluator_t binary_oper(binary_func_t func, evaluator_t lhs, evaluator_t rhs)
{
    return [=](context_t& context)
    {
        return func(lhs(context), rhs(context));
    };
}

evaluator_t unary_oper(unary_func_t func, evaluator_t item)
{
    return [=](context_t& context)
    {
        return func(item(context));
    };
}

evaluator_t number(double value)
{
    return [=](context_t&)
    {
        return value;
    };
}

evaluator_t variable(std::string name)
{
    return [=](context_t& context)
    {
        auto it = context.vars.find(name);
        core::ensures(it != context.vars.end(), [&]() { return core::str("Undefined var '", name, "'"); });
        return it->second;
    };
}

evaluator_t assignment(std::string name, evaluator_t item)
{
    return [=](context_t& context)
    {
        const double v = item(context);
        context.vars[name] = v;
        return v;
    };
}

evaluator_t function(std::string name, std::vector<evaluator_t> evaluators)
{
    return [=](context_t& context)
    {
        auto it = context.funcs.find(name);
        core::ensures(it != context.funcs.end(), [&]() { return core::str("Undefined function '", name, "'"); });
        const auto values = evaluators | sq::transform([&](const evaluator_t& e) { return e(context); }) | sq::to_vector();
        return it->second(values);
    };
}

void split(core::cstring_view text, char separator, const core::action<core::cstring_view>& callback)
{
    int level = 0;
    auto prev = text.begin();
    for (auto it = text.begin(); it != text.end(); ++it)
    {
        if (*it == '(')
        {
            ++level;
        }
        else if (*it == ')')
        {
            --level;
        }

        if (level == 0 && *it == separator)
        {
            callback(core::cstring_view{ prev, it } | sq::trim_while(core::is_space));
            prev = it + 1;
        }
    }

    callback(core::cstring_view{ prev, text.end() } | sq::trim_while(core::is_space));
}

evaluator_t parse_expr(core::cstring_view text)
{
    text = text | sq::trim_while(core::is_space);

    core::ensures(valid_brackets(text), [&]() { return core::str("Invalid brackets in '", text, "'"); });
    core::ensures(!text.empty(), []() { return "cannot parse empty expression"; });

    if (text.front() == '(' && text.back() == ')')
    {
        return parse_expr(text | sq::slice(1, -1));
    }

    if (auto prefix_oper = find_prefix_oper(text); prefix_oper)
    {
        const auto[op_info, rhs_expr] = *prefix_oper;
        return unary_oper(op_info.op, parse_expr(rhs_expr));
    }

    if (auto infix_oper = find_infix_oper(text); infix_oper)
    {
        const auto[lhs_expr, op_info, rhs_expr] = *infix_oper;
        return op_info.op
            ? binary_oper(op_info.op, parse_expr(lhs_expr), parse_expr(rhs_expr))
            : assignment(lhs_expr, parse_expr(rhs_expr));
    }    

    if (auto val = core::try_parse<double>(text); val)
    {
        return number(*val);
    }

    if (auto it = core::find(text, '('); !it.empty())
    {
        auto[name, args] = text | sq::split(it.begin());
        std::vector<evaluator_t> evaluators;
        split(args | sq::slice(1, -1), ',', [&](core::cstring_view sub) { evaluators.push_back(parse_expr(sub)); });
        return function(name | sq::trim_while(core::is_space), std::move(evaluators));
    }

    return variable(text);
}

evaluator_t parse(core::cstring_view text)
{
    std::vector<evaluator_t> evaluators;
    split(text, ';', [&](core::cstring_view sub)
    {
        if (!sub.empty())
        {
            evaluators.push_back(parse_expr(sub));
        }
    });

    return [=](context_t& context)
    {
        double result = 0.0;
        for (const evaluator_t& evaluator : evaluators)
        {
            result = evaluator(context);
        }
        return result;
    };
}

double eval(core::cstring_view text, context_t& context)
{
    return parse(text)(context);
}

func_t from_unary_func(unary_func_t func)
{
    return [=](const std::vector<double>& args)
    {
        core::ensures(args.size() == 1, "one argument expected");
        return func(args.at(0));
    };
}

func_t from_binary_func(binary_func_t func)
{
    return [=](const std::vector<double>& args)
    {
        core::ensures(args.size() == 2, "two argument expected");
        return func(args.at(0), args.at(1));
    };
}

void run(core::program_args args)
{
    context_t context;
    context.vars["pi"] = math::pi;
    context.vars["true"] = 1.0;
    context.vars["false"] = 0.0;

    context.funcs["pow"] = from_binary_func(math::pow);

    context.funcs["sin"] = from_unary_func(math::sin);
    context.funcs["cos"] = from_unary_func(math::cos);
    context.funcs["tan"] = from_unary_func(math::tan);
    context.funcs["cot"] = from_unary_func(math::cot);
    context.funcs["floor"] = from_unary_func(math::floor);
    context.funcs["ceil"] = from_unary_func(math::ceil);
    context.funcs["abs"] = from_unary_func(math::abs);
    context.funcs["exp"] = from_unary_func(math::exp);
    context.funcs["max"] = core::max_value;
    context.funcs["min"] = core::min_value;
    context.funcs["sum"] = core::sum;

    while (true)
    {
        std::cout << ">>> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty())
        {
            return;
        }

        try
        {
            std::cout << eval(core::c_str(line), context) << std::endl;
        }
        catch (std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }
}

int main(int argc, char** argv)
{
    try
    {
        run(core::create_program_args(argc, argv));
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    return 0;
}
