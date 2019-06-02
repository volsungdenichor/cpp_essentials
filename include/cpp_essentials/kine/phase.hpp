#pragma once

#include <cpp_essentials/kine/animation.core.hpp>

namespace cpp_essentials::kine
{

template <class T>
class phase;

template <class T>
using phase_ptr = std::shared_ptr<const phase<T>>;


template <class T>
class phase
{
public:
    phase(duration_type duration)
        : _duration { duration }
    {
    }

    duration_type duration() const
    {
        return _duration;
    }

    virtual T value(time_type time) const = 0;

    T operator ()(time_type time) const
    {
        return value(time);
    }

    float duration_ratio(time_type time) const
    {
        return time / duration();
    }

    virtual T start_value() const
    {
        return value(duration_type {});
    }

    virtual T end_value() const
    {
        return value(duration());
    }

    T wrapped_value(time_type time, time_type inflection_point = {}) const
    {
        return value(wrap(time, duration(), inflection_point));
    }

protected:
    duration_type _duration;
};

} /* namespace cpp_essentials::kine */
