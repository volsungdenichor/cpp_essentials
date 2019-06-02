#pragma once

#include <memory>

#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/assertions.hpp>

namespace cpp_essentials::kine
{

using time_type = float;
using duration_type = float;

inline time_type wrap(time_type time, duration_type duration, time_type inflection_point = {})
{
    return time > duration
        ? inflection_point + std::fmod(time, duration - inflection_point)
        : time;
}

} /* namespace cpp_essentials::kine */