#pragma once

#include <cpp_essentials/kine/sequence_phase.hpp>
#include <cpp_essentials/kine/constant.hpp>
#include <cpp_essentials/kine/gradual.hpp>

namespace cpp_essentials::kine
{

template <class T>
class sequence
{
public:
    sequence(T init_value = {}, std::vector<phase_ptr<T>> phases = {})
        : _sequence{ std::make_shared<sequence_phase<T>>(std::move(init_value), std::move(phases)) }
    {
    }

    sequence(phase_ptr<T> phase)
        : sequence{ phase->start_value() }
    {
        _sequence->append(phase);
    }

    sequence(const std::vector<phase_ptr<T>>& phases)
        : sequence{ phases[0]->start_value(), phases }
    {
    }

    phase_ptr<T> as_phase() const
    {
        return _sequence;
    }

    duration_type duration() const
    {
        return _sequence->duration();
    }

    T operator ()(time_type time) const
    {
        return value(time);
    }

    T value(time_type time) const
    {
        return _sequence->value(time);
    }

    T start_value() const
    {
        return _sequence->start_value();
    }

    T end_value() const
    {
        return _sequence->end_value();
    }

    sequence& then(phase_ptr<T> phase)
    {
        _sequence->append(phase);
        return *this;
    }

    sequence& then(const sequence& next)
    {
        _sequence->append(next._sequence);
        return *this;
    }

    sequence& then_hold(T value, duration_type duration)
    {
        return then(constant(std::move(value), duration));
    }

    sequence& then_hold(duration_type duration)
    {
        return then_hold(end_value(), duration);
    }

    sequence& then_ramp(T start_value, T end_value, duration_type duration, ease_function ease = ease::none)
    {
        return then(gradual(std::move(start_value), std::move(end_value), duration, std::move(ease)));
    }

    sequence& then_ramp(T value, duration_type duration, ease_function ease = ease::none)
    {
        return then_ramp(end_value(), std::move(value), duration, std::move(ease));
    }

private:
    std::shared_ptr<sequence_phase<T>> _sequence;
};

template <class T>
sequence<T> create_sequence(T value)
{
    return { std::move(value) };
}

} /* namespace cpp_essentials::kine */
