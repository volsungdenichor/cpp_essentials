#pragma once

#include <functional>

#include <cpp_essentials/kine/phase.hpp>
#include <cpp_essentials/kine/ease.hpp>

namespace cpp_essentials::kine
{

template <class T>
class ramp_phase : public phase<T>
{
public:
    ramp_phase(duration_type duration, T start_value, T end_value, ease_function ease)
        : phase<T>{ duration }
        , _start_value{ std::move(start_value) }
        , _end_value{ std::move(end_value) }
        , _ease{ std::move(ease) }
    {
    }

    T value(time_type time) const override
    {
        return static_cast<T>(core::lerp(_ease(phase<T>::duration_ratio(time)), _start_value, _end_value));
    }

    T start_value() const override
    {
        return _start_value;
    }

    T end_value() const override
    {
        return _end_value;
    }

private:
    T _start_value;
    T _end_value;
    ease_function _ease;
};

template <class T>
phase_ptr<T> ramp(T start_value, T end_value, duration_type duration, ease_function ease = ease::none)
{
    return std::make_shared<ramp_phase<T>>(duration, std::move(start_value), std::move(end_value), std::move(ease));
}

} /* namespace cpp_essentials::kine */
