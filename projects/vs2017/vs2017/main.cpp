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
#include <cpp_essentials/core/debug_utils.hpp>

#include <Windows.h>

using namespace cpp_essentials;

using argument_extractor = std::function<void(std::ostream&, int, std::string_view)>;

struct format_error : std::runtime_error
{
    format_error(std::string message)
        : std::runtime_error{ std::move(message) }
    {
    }
};

std::string_view make_string_view(std::string_view::iterator b, std::string_view::iterator e)
{
    if (b < e)
        return { std::addressof(*b), std::string_view::size_type(e - b) };
    else
        return {};
}

void format_text(std::ostream& os, std::string_view txt)
{
    os << txt;
}

int parse_int(std::string_view txt)
{
    int result = 0;
    for (char c : txt)
    {
        result = result * 10 + (c - '0');
    }
    return result;
}

void apply_format_spec(std::ostream& os, std::string_view fmt)
{
    using character_map = std::map<char, std::function<void()>>;
    const character_map alignment
    {
        { '<', [&]() { os << std::left; } },
        { '>', [&]() { os << std::right; } },
        { '^', [&]() { } },
        { '=', [&]() { } },
    };

    const character_map sign
    {
        { '+', [&]() { os << std::showpos; } },
        { '-', [&]() { } },
    };

    const character_map type
    {
        { 'f', [&]() { os << std::fixed; } },
        { 'F', [&]() { os << std::fixed; } },
        { 'g', [&]() { } },
        { 'G', [&]() { } },
        { 'd', [&]() { os << std::dec; } },
        { 'e', [&]() { os << std::scientific; } },
        { 'E', [&]() { os << std::scientific << std::uppercase; } },
        { 'o', [&]() { os << std::oct; } },
        { 'x', [&]() { os << std::hex; } },
        { 'X', [&]() { os << std::hex << std::uppercase; } },
    };

    char fill = '\0';
    
    if (fmt.size() >= 2)
    {
        if (alignment.count(fmt[1]))
        {
            fill = fmt[0];
            fmt = make_string_view(fmt.begin() + 1, fmt.end());
        }
    }

    const auto handle_front = [&](const character_map& map)
    {
        if (fmt.empty())
            return;
        
        if (auto it = map.find(fmt.front()); it != map.end())
        {
            it->second();
            fmt = make_string_view(fmt.begin() + 1, fmt.end());
        }
    };

    const auto handle_back = [&](const character_map& map)
    {
        if (fmt.empty())
            return;

        if (auto it = map.find(fmt.back()); it != map.end())
        {
            it->second();
            fmt = make_string_view(fmt.begin(), fmt.end() - 1);
        }
    };

    os << std::fixed;

    handle_front(alignment);
    handle_front(sign);
    handle_back(type);

    if (!fmt.empty() && fmt.front() == '0')
    {
        if (!fill)
            fill = '0';
        fmt = make_string_view(fmt.begin() + 1, fmt.end());
    }

    const auto dot = std::find(fmt.begin(), fmt.end(), '.');
    const auto width = parse_int(make_string_view(fmt.begin(), dot));
    const auto precision = dot != fmt.end()
        ? parse_int(make_string_view(dot + 1, fmt.end()))
        : 0;

    if (fill)
        os << std::setfill(fill);
    os << std::setw(width) << std::setprecision(precision);
}

template <class T>
void write_arg(std::ostream& os, std::string_view fmt, const T& item)
{
    apply_format_spec(os, fmt);
    os << item;
}

void write_args(std::ostream& os, int index, std::string_view fmt)
{
    throw format_error{ "Invalid index" };
}

template <class T, class... Args>
void write_args(std::ostream& os, int index, std::string_view fmt, const T& arg, const Args&... args)
{
    if (index == 0)
        write_arg(os, fmt, arg);
    else
        write_args(os, index - 1, fmt, args...);
}

void format_arg(std::ostream& os, std::string_view fmt, int arg_index, const argument_extractor& arg_extractor)
{
    const auto colon = std::find(fmt.begin(), fmt.end(), ':');
    const auto index_part = make_string_view(fmt.begin(), colon);
    const auto fmt_part = make_string_view(colon != fmt.end() ? colon + 1 : colon, fmt.end());

    const auto actual_index = !index_part.empty()
        ? parse_int(index_part)
        : arg_index;

    arg_extractor(os, actual_index, fmt_part);
}

void do_format(std::ostream& os, std::string_view fmt, int arg_index, const argument_extractor& arg_extractor)
{
    const auto bracket = std::find_if(fmt.begin(), fmt.end(), [](char c) { return c == '{' || c == '}'; });
    if (bracket == fmt.end())
    {
        return format_text(os, fmt);
    }
    else if (bracket + 1 != fmt.end() && bracket[0] == bracket[1])
    {
        format_text(os, make_string_view(fmt.begin(), bracket + 1));
        return do_format(os, make_string_view(bracket + 2, fmt.end()), arg_index, arg_extractor);
    }
    else if (bracket[0] == '{')
    {
        const auto closing_bracket = std::find(bracket + 1, fmt.end(), '}');
        if (closing_bracket == fmt.end())
        {
            throw format_error{ "unclosed bracket" };
        }
        format_text(os, make_string_view(fmt.begin(), bracket));
        format_arg(os, make_string_view(bracket + 1, closing_bracket), arg_index, arg_extractor);
        return do_format(os, make_string_view(closing_bracket + 1, fmt.end()), arg_index + 1, arg_extractor);
    }
    throw format_error{ "unexpected opening bracket" };
}



template <class... Args>
std::string format(std::string_view fmt, const Args&... args)
{
    const argument_extractor arg_extractor = [&](std::ostream& s, int index, std::string_view f)
    {
        write_args(s, index, f, args...);
    };

    std::stringstream ss;
    do_format(ss, fmt, 0, arg_extractor);
    return ss.str();
}

template <class... Args>
std::ostream& print(std::ostream& os, std::string_view fmt, const Args&... args)
{
    return os << format(fmt, args...);
}

template <class... Args>
std::ostream& println(std::ostream& os, std::string_view fmt, const Args&... args)
{
    return os << format(fmt, args...) << '\n';
}

template <class... Args>
void print(std::string_view fmt, const Args&... args)
{
    print(std::cout, fmt, args...);
}

template <class... Args>
void println(std::string_view fmt, const Args&... args)
{
    println(std::cout, fmt, args...);
}

struct format_proxy_t
{
    std::string_view fmt;

    template <class... Args>
    std::string operator ()(const Args&... args) const
    {
        return format(fmt, args...);
    }
};

auto operator ""_format(const char* text, std::size_t size) -> format_proxy_t
{
    return { { text, size } };
}

void run(core::program_args args)
{
    println("'{:06.2f}'", 31.415);
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
