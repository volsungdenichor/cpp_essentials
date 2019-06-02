#pragma once

#include <cpp_essentials/kine/phase.hpp>

namespace cpp_essentials::kine
{

template <class T>
class looped_phase : public phase<T>
{
public:
    looped_phase(phase_ptr<T> inner, float count, time_type inflection_point = {})
        : phase<T>{ inner->duration() * count }
        , _inner{ std::move(inner) }
        , _inflection_point{ inflection_point }
    {
    }

    T value(time_type time) const override
    {
        return _inner->wrapped_value(time, _inflection_point);
    }

    T start_value() const override
    {
        return _inner->start_value();
    }

    T end_value() const override
    {
        return _inner->wrapped_value(phase<T>::duration());
    }

private:
    phase_ptr<T> _inner;
    time_type _inflection_point;
};

template <class T>
phase_ptr<T> loop(phase_ptr<T> inner, float count, time_type inflection_point = {})
{
    return std::make_shared<looped_phase<T>>(std::move(inner), count, inflection_point);
}

} /* namespace cpp_essentials::kine */
