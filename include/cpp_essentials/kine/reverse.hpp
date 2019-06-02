#pragma once

#include <cpp_essentials/kine/phase.hpp>

namespace cpp_essentials::kine
{

template <class T>
class reversed_phase : public phase<T>
{
public:
    reversed_phase(phase_ptr<T> inner)
        : phase<T>{ inner->duration() }
        , _inner{ std::move(inner) }
    {}

    T value(time_type time) const override
    {
        return _inner->value(_inner->duration() - time);
    }

    T start_value() const override
    {
        return _inner->end_value();
    }

    T end_value() const
    {
        return _inner->start_value();
    }

private:
    phase_ptr<T> _inner;
};

template <class T>
phase_ptr<T> reverse(phase_ptr<T> inner)
{
    return std::make_shared<reversed_phase<T>>(std::move(inner));
}

} /* namespace cpp_essentials::kine */
