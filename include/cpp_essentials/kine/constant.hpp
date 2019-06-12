#pragma once

#include <cpp_essentials/kine/phase.hpp>

namespace cpp_essentials::kine
{

template <class T>
class constant_phase : public phase<T>
{
public:
    constant_phase(duration_type duration, T value)
        : phase<T>{ duration }
        , _value { std::move(value) }
    {
    }

    T value(time_type) const override
    {
        return _value;
    }

private:
    T _value;
};

template <class T>
phase_ptr<T> constant(T value, duration_type duration)
{
    return std::make_shared<constant_phase<T>>(duration, std::move(value));
}

} /* namespace cpp_essentials::kine */
