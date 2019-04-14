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

static constexpr detail::is_space_fn is_space = {};
static constexpr detail::is_digit_fn is_digit = {};
static constexpr detail::is_blank_fn is_blank = {};
static constexpr detail::is_alpha_fn is_alpha = {};
static constexpr detail::is_alnum_fn is_alnum = {};
static constexpr detail::is_upper_fn is_upper = {};
static constexpr detail::is_lower_fn is_lower = {};

static constexpr detail::to_lower_fn to_lower = {};
static constexpr detail::to_upper_fn to_upper = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP */