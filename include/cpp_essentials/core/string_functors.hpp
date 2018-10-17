#ifndef CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP
#define CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP

#pragma once

#include <locale>

namespace cpp_essentials::core
{

namespace detail
{

struct is_space_t
{
    bool operator ()(char ch) const
    {
        return std::isspace(ch, std::locale{});
    }
};

struct is_digit_t
{
    bool operator ()(char ch) const
    {
        return std::isdigit(ch, std::locale{});
    }
};

struct is_blank_t
{
    bool operator ()(char ch) const
    {
        return std::isblank(ch, std::locale{});
    }
};

struct is_alpha_t
{
    bool operator ()(char ch) const
    {
        return std::isalpha(ch, std::locale{});
    }
};

struct is_alnum_t
{
    bool operator ()(char ch) const
    {
        return std::isalnum(ch, std::locale{});
    }
};

struct is_upper_t
{
    bool operator ()(char ch) const
    {
        return std::isupper(ch, std::locale{});
    }
};

struct is_lower_t
{
    bool operator ()(char ch) const
    {
        return std::islower(ch, std::locale{});
    }
};


struct to_upper_t
{
    char operator ()(char ch) const
    {
        return std::toupper(ch, std::locale{});
    }
};

struct to_lower_t
{
    char operator ()(char ch) const
    {
        return std::toupper(ch, std::locale{});
    }
};

} /* namespace detail */

static constexpr detail::is_space_t is_space = {};
static constexpr detail::is_digit_t is_digit = {};
static constexpr detail::is_blank_t is_blank = {};
static constexpr detail::is_alpha_t is_alpha = {};
static constexpr detail::is_alnum_t is_alnum = {};
static constexpr detail::is_upper_t is_upper = {};
static constexpr detail::is_lower_t is_lower = {};

static constexpr detail::to_lower_t to_lower = {};
static constexpr detail::to_upper_t to_upper = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_STRING_FUNCTORS_HPP */