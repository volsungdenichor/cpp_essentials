#ifndef CPP_ESSENTIALS_PROC_SEED_HPP_
#define CPP_ESSENTIALS_PROC_SEED_HPP_

#pragma once

#include <random>
#include <optional>

namespace cpp_essentials::proc
{

class seed_t
{
public:
    using value_type = std::random_device::result_type;

    seed_t()
        : _value{ std::nullopt }
    {
    }

    explicit seed_t(value_type value)
        : _value{ value }
    {
    }

    template <class Generator = std::default_random_engine>
    Generator to_generator() const
    {
        static std::random_device rd{};

        return Generator{ _value ? *_value : rd() };
    }

private:
    std::optional<value_type> _value;
};

inline seed_t seed(seed_t::value_type value)
{
    return seed_t{ value };
}

} /* namespace cpp_essentials::proc */

#endif /* CPP_ESSENTIALS_PROC_SEED_HPP_ */
