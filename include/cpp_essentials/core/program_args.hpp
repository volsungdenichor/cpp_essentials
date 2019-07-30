#ifndef CPP_ESSENTIALS_CORE_PROGRAM_ARGS_HPP_
#define CPP_ESSENTIALS_CORE_PROGRAM_ARGS_HPP_

#pragma once

#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/string_views.hpp>

namespace cpp_essentials::core
{

namespace detail
{

auto do_create_program_args(int argc, char** argv)
{
    return core::views::map(core::make_range(argv, argv + argc), core::c_str);
}

using program_args = decltype(do_create_program_args(0, (char**)nullptr));

struct create_program_args_fn
{
    program_args operator ()(int argc, char** argv) const
    {
        return do_create_program_args(argc, argv);
    }
};

} /* namespace detail */

using detail::program_args;
static constexpr detail::create_program_args_fn create_program_args = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_PROGRAM_ARGS_HPP_ */