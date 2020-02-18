#ifndef CPP_ESSENTIALS_CORE_FORMAT_HPP_
#define CPP_ESSENTIALS_CORE_FORMAT_HPP_

#include <ostream>
#include <iomanip>
#include <functional>
#include <unordered_map>

#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/slice.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/containers.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/string_functors.hpp>

namespace cpp_essentials::core
{

class format_error : public std::runtime_error
{
public:
    format_error(const std::string& message)
        : std::runtime_error{ std::string { "format: " } + message }
    {
    }
};

namespace detail
{

struct locale_guard
{
    explicit locale_guard(std::ostream& os)
        : _os{ os }
        , _locale{ _os.getloc() }
    {
    }

    ~locale_guard()
    {
        _os.imbue(_locale);
    }

    std::ostream& _os;
    std::locale _locale;
};

struct format_guard
{
    explicit format_guard(std::ostream& os)
        : _os{ os }
        , _format{ _os.flags() }
    {
    }

    ~format_guard()
    {
        _os.flags(_format);
    }

    std::ostream& _os;
    std::ios::fmtflags _format;
};

using key_value_func = std::function<void(cstr_cview, cstr_cview)>;

void parse_term(cstr_cview text, key_value_func func)
{
    if (text.empty())
    {
        return;
    }

    auto sep = find(text, '=');
    if (!sep.empty())
    {
        auto key = trim(make_range(text.begin(), sep.begin()));
        auto value = trim(core::make_range(sep.begin() + 1, text.end()));
        func(key, value);
    }
    else
    {
        func(trim(text), {});
    }
}

void parse_format(cstr_cview text, key_value_func func)
{
    auto prev = text.begin();

    for (auto it = text.begin(); it != text.end(); ++it)
    {
        if (*it == '|')
        {
            parse_term(trim(make_range(prev, it)), func);
            prev = it + 1;
        }
    }

    parse_term(trim(make_range(prev, text.end())), func);
}

struct format_modifier_handler
{
    std::ostream& os;

    format_modifier_handler(std::ostream& os)
        : os{ os }
    {
    }

    void format(cstr_cview value) const
    {
        const auto is_placeholder = [](char ch) { return ch == '0'; };
        const auto is_dot = [](char ch) { return ch == '.'; };

        const auto integer = views::take_while(value, is_placeholder);
        value = views::drop(value, integer.size());

        const auto sep = views::take_while(value, is_dot);
        value = views::drop(value, sep.size());

        const auto fractional = views::take_while(value, is_placeholder);
        value = views::drop(value, fractional.size());

        EXPECTS(value.empty(), "extra format specifier");
        EXPECTS(sep.size() <= 1, "unexpected dot");

        if (sep.empty())
        {
            os
                << std::fixed << std::internal
                << std::setfill('0')
                << std::setw(integer.size());
        }
        else
        {
            os
                << std::fixed << std::internal
                << std::setfill('0')
                << std::setw(integer.size() + fractional.size() + 1)
                << std::setprecision(fractional.size());
        }
    }

    void operator ()(cstr_cview key, cstr_cview value) const
    {
        using namespace literals;
        if (key == "format"_s || key == "fmt"_s)
        {
            format(value);
        }
        else if (key == "width"_s)
        {
            os << std::setw(parse<int>(value));
        }
        else if (key == "fill"_s)
        {
            os << std::setfill(value.front());
        }
        else if (key == "precision"_s)
        {
            os << std::setprecision(parse<int>(value));
        }
        else if (key == "left"_s)
        {
            os << std::left;
        }
        else if (key == "right"_s)
        {
            os << std::right;
        }
        else if (key == "internal"_s)
        {
            os << std::internal;
        }
        else if (key == "fixed"_s)
        {
            os << std::fixed;
        }
        else if (key == "scientific"_s)
        {
            os << std::scientific;
        }
        else if (key == "dec"_s)
        {
            os << std::dec;
        }
        else if (key == "hex"_s)
        {
            os << std::hex;
        }
        else if (key == "oct"_s)
        {
            os << std::oct;
        }
        else if (key == "boolalpha"_s)
        {
            os << std::boolalpha;
        }
        else if (key == "uppercase"_s)
        {
            os << std::uppercase;
        }
        else if (key == "showpoint"_s)
        {
            os << std::showpoint;
        }
        else if (key == "showpos"_s)
        {
            os << std::showpos;
        }
        else if (key == "showbase"_s)
        {
            os << std::showbase;
        }
        else
        {
            THROW(format_error{ "unrecognized format modifier" });
        }
    }
};

template <class T>
void write_value(std::ostream& os, cstr_cview fmt, const T& value)
{
    const auto save_format = format_guard{ os };
    (void)save_format;

    if (!fmt.empty())
    {
        parse_format(fmt, format_modifier_handler{ os });
    }

    os << value;
}


template <class Arg>
void write_argument(std::ostream& os, cstr_cview fmt, int index, const Arg& arg)
{
    if (index == 0)
    {
        write_value(os, fmt, arg);
    }
    else
    {
        THROW(format_error{ "argument index out of range" });
    }
}

template <class Arg, class... Args>
void write_argument(std::ostream& os, cstr_cview fmt, int index, const Arg& arg, const Args&... args)
{
    if (index == 0)
    {
        write_value(os, fmt, arg);
    }
    else
    {
        write_argument(os, fmt, index - 1, args...);
    }
}

inline void write_argument(std::ostream& /*os*/, cstr_cview /*fmt*/, int /*index*/)
{
    THROW(format_error{ "argument index out of range" });
}

inline std::tuple<int, cstr_cview> parse_field(cstr_cview fmt)
{
    auto digits_fmt = views::take_while(views::drop_while(fmt, core::is_blank), core::is_digit);

    int index = !digits_fmt.empty()
        ? core::parse<int>(digits_fmt)
        : -1;

    return std::make_tuple(index, core::find(core::return_next_end, fmt, ':'));
}

template <class... Args>
void handle_item(std::ostream& os, int argument_index, cstr_cview fmt, const Args&... args)
{
    auto[field_index, field_fmt] = parse_field(fmt);

    if (field_index == -1)
    {
        field_index = argument_index;
    }

    write_argument(os, field_fmt, field_index, args...);
}

template <class... Args>
void do_format(std::ostream& os, cstr_cview fmt, const Args&... args)
{
    int argument_index = 0;

    for (auto it = fmt.begin(); it != fmt.end(); ++it)
    {
        if (it[0] == '{')
        {
            if (it[1] == '{')
            {
                os << '{';
                ++it;
            }
            else
            {
                auto e = std::find(it + 1, fmt.end(), '}');

                if (e == fmt.end())
                {
                    THROW(format_error{ "unclosed bracket" });
                }

                handle_item(os, argument_index++, cstr_cview(it + 1, e), args...);

                it = e;
            }
        }
        else if (it[0] == '}')
        {
            if (it[1] != '}')
            {
                THROW(format_error{ "unexpected closing bracket" });
            }

            os << '}';
            ++it;
        }
        else
        {
            os << *it;
        }

    }
}

struct format_fn
{
    template <class... Args>
    std::ostream& operator ()(std::ostream& os, const std::locale& locale, std::string_view fmt, const Args&... args) const
    {
        const auto save_locale = detail::locale_guard{ os };
        (void)save_locale;

        const auto save_format = detail::format_guard{ os };
        (void)save_format;

        os.imbue(locale);
        do_format(os, c_str(fmt), args...);
        return os;
    }

    template <class... Args>
    std::ostream& operator ()(std::ostream& os, std::string_view fmt, const Args&... args) const
    {
        do_format(os, c_str(fmt), args...);
        return os;
    }

    template <class... Args>
    std::string operator ()(const std::locale& locale, std::string_view fmt, const Args&... args) const
    {
        std::stringstream ss;
        (*this)(ss, locale, fmt, args...);
        return ss.str();
    }

    template <class... Args>
    std::string operator ()(std::string_view fmt, const Args&... args) const
    {
        std::stringstream ss;
        (*this)(ss, fmt, args...);
        return ss.str();
    }
};

static constexpr auto format = format_fn{};

struct print_fn
{
    template <class... Args>
    std::ostream& operator ()(const std::locale& locale, std::string_view fmt, const Args&... args) const
    {
        return format(std::cout, locale, fmt, args...);
    }

    template <class... Args>
    std::ostream& operator ()(std::string_view fmt, const Args&... args) const
    {
        return format(std::cout, fmt, args...);
    }
};

static constexpr auto print = print_fn{};

struct println_fn
{
    template <class... Args>
    std::ostream& operator ()(const std::locale& locale, std::string_view fmt, const Args&... args) const
    {
        return print(locale, fmt, args...) << std::endl;
    }

    template <class... Args>
    std::ostream& operator ()(std::string_view fmt, const Args&... args) const
    {
        return print(fmt, args...) << std::endl;
    }
};

static constexpr auto println = println_fn{};

} /* namespace detail */

using detail::format;
using detail::print;
using detail::println;

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FORMAT_HPP_ */

