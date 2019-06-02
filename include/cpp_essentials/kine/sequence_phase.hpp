#pragma once

#include <vector>

#include <cpp_essentials/kine/phase.hpp>

namespace cpp_essentials::kine
{

template <class T>
class sequence_phase : public phase<T>
{
public:
    sequence_phase(T init_value = {}, std::vector<phase_ptr<T>> phases = {})
        : phase<T>{ duration_type {} }
        , _init_value{ std::move(init_value) }
        , _phases{ std::move(phases) }
    {
        update_duration();
    }

    T value(time_type time) const override
    {
        if (time < time_type{})
        {
            return _init_value;
        }
        else if (time >= phase<T>::duration())
        {
            return end_value();
        }

        for (const auto& phase : _phases)
        {
            if (phase->duration() < time)
            {
                time -= phase->duration();
            }
            else
            {
                return phase->value(time);
            }
        }

        return end_value();
    }

    T start_value() const override
    {
        return _phases.empty()
            ? _init_value
            : _phases.front()->start_value();
    }

    T end_value() const override
    {
        return _phases.empty()
            ? _init_value
            : _phases.back()->end_value();
    }

    void append(phase_ptr<T> phase)
    {
        _phases.push_back(phase);
        update_duration();
    }

    void append(const std::vector<phase_ptr<T>>& phases)
    {
        _phases.insert(_phases.end(), phases.begin(), phases.end());
        update_duration();
    }

public:
    phase_ptr<T> get_phase(time_type time) const
    {
        EXPECTS(!_phases.empty());

        if (time < time_type{})
        {
            return _phases.front();
        }
        else if (time > phase<T>::duration())
        {
            return _phases.back();
        }

        for (const auto& phase : _phases)
        {
            if (phase->duration() < time)
            {
                time -= phase->duration();
            }
            else
            {
                return phase;
            }
        }

        return _phases.back();
    }

    void update_duration()
    {
        phase<T>::_duration = core::accumulate(_phases, duration_type{}, [](duration_type total, const phase_ptr<T>& item) { return total + item->duration(); });
    }

    T _init_value;
    std::vector<phase_ptr<T>> _phases;
};

} /* namespace cpp_essentials::kine */
