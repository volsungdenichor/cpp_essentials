#ifndef CPP_ESSENTIALS_CORE_EVENT_HPP_
#define CPP_ESSENTIALS_CORE_EVENT_HPP_

#pragma once

#include <vector>
#include <cpp_essentials/core/function_defs.hpp>

namespace cpp_essentials::core
{

template <class... Args>
class event
{
public:
    using action_type = action<Args...>;

    event() = default;

    event(action_type func)
    {
        add(std::move(func));
    }

    void operator +=(action_type func)
    {
        add(std::move(func));
    }

    void operator =(action_type func)
    {
        _collection.clear();
        add(std::move(func));
    }

    void operator ()(const Args&... args) const
    {
        for (const auto& item : _collection)
        {
            item(args...);
        }
    }

private:
    void add(action_type&& func)
    {
        _collection.push_back(std::move(func));
    }

    std::vector<action_type> _collection;
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_EVENT_HPP_ */