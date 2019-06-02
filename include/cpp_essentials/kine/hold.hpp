#pragma once

#include <cpp_essentials/kine/phase.hpp>

namespace cpp_essentials::kine
{

template <class T>
class held_phase : public phase<T>
{
public:
    held_phase(duration_type duration, T value)
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
phase_ptr<T> hold(T value, duration_type duration)
{
    return std::make_shared<held_phase<T>>(duration, std::move(value));
}

} /* namespace cpp_essentials::kine */
