#ifndef CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP
#define CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP

#pragma once

#include <locale>

namespace cpp_essentials::core
{

namespace detail
{

struct is_space_fn
{
    bool operator ()(char ch) const
    {
        return std::isspace(ch, std::locale{});
    }
};

struct is_digit_fn
{
    bool operator ()(char ch) const
    {
        return std::isdigit(ch, std::locale{});
    }
};

struct is_blank_fn
{
    bool operator ()(char ch) const
    {
        return std::isblank(ch, std::locale{});
    }
};

struct is_alpha_fn
{
    bool operator ()(char ch) const
    {
        return std::isalpha(ch, std::locale{});
    }
};

struct is_alnum_fn
{
    bool operator ()(char ch) const
    {
        return std::isalnum(ch, std::locale{});
    }
};

struct is_upper_fn
{
    bool operator ()(char ch) const
    {
        return std::isupper(ch, std::locale{});
    }
};

struct is_lower_fn
{
    bool operator ()(char ch) const
    {
        return std::islower(ch, std::locale{});
    }
};


struct to_upper_fn
{
    char operator ()(char ch) const
    {
        return std::toupper(ch, std::locale{});
    }
};

struct to_lower_fn
{
    char operator ()(char ch) const
    {
        return std::tolower(ch, std::locale{});
    }
};

} /* namespace detail */

static constexpr auto is_space = detail::is_space_fn{};
static constexpr auto is_digit = detail::is_digit_fn{};
static constexpr auto is_blank = detail::is_blank_fn{};
static constexpr auto is_alpha = detail::is_alpha_fn{};
static constexpr auto is_alnum = detail::is_alnum_fn{};
static constexpr auto is_upper = detail::is_upper_fn{};
static constexpr auto is_lower = detail::is_lower_fn{};

static constexpr auto to_lower = detail::to_lower_fn{};
static constexpr auto to_upper = detail::to_upper_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP */