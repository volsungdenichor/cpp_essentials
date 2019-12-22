#ifndef CPP_ESSENTIALS_CORE_LAZY_EVALUATED_HPP_
#define CPP_ESSENTIALS_CORE_LAZY_EVALUATED_HPP_

#pragma once

#include <functional>
#include <variant>

#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

template <class T>
class lazy_evaluated
{
public:
    using value_type = T;
    using generator_type = std::function<value_type()>;

    lazy_evaluated(value_type value)
        : _data{ std::move(value) }
    {
    }

    lazy_evaluated(generator_type value)
        : _data{ std::move(value) }
    {
    }

    template <class U>
    lazy_evaluated(U&& value)
        : lazy_evaluated{ std::move(value), std::is_invocable<U>{} }
    {
    }

    const value_type& operator *() const
    {
        if (std::holds_alternative<generator_type>(_data))
        {
            _data.emplace<value_type>(std::get<generator_type>(_data)());
        }
        return std::get<value_type>(_data);
    }

private:
    template <class U>
    lazy_evaluated(U&& value, std::false_type)
        : lazy_evaluated{ value_type(std::move(value)) }
    {
    }

    template <class U>
    lazy_evaluated(U&& value, std::true_type)
        : lazy_evaluated{ generator_type(std::move(value)) }
    {
    }
    
    mutable std::variant<generator_type, value_type> _data;

};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_LAZY_EVALUATED_HPP_ */
