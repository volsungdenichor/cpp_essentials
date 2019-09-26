#ifndef CPP_ESSENTIALS_CORE_EVENT_AGGREGATOR_HPP_
#define CPP_ESSENTIALS_CORE_EVENT_AGGREGATOR_HPP_

#pragma once

#include <map>
#include <mutex>
#include <typeindex>

#include <cpp_essentials/core/task_queue.hpp>
#include <cpp_essentials/core/container_helpers.hpp>

namespace cpp_essentials::core
{

class event_aggregator
{
public:
    using sub_id = int;
    using handler_t = action<const void*>;

    event_aggregator(std::chrono::milliseconds timeout = std::chrono::milliseconds{ 10 })
        : _map{}
        , _next_id{ 0 }
        , _queue{ timeout }
        , _mutex{}
    {
    }

    event_aggregator(event_aggregator&) = delete;
    event_aggregator(event_aggregator&&) = delete;

    template <class E>
    sub_id subscribe(sub_id id, action<E> event_handler)
    {
        handler_t handler = [=](const void* e)
        {
            event_handler(*static_cast<const E*>(e));
        };

        std::unique_lock<std::mutex> lock{ _mutex };
        _map.emplace(get_type<E>(), sub_info{ std::move(handler), id });
        return id;
    }

    template <class E>
    sub_id subscribe(action<E> event_handler)
    {
        return subscribe(acquire_id(), std::move(event_handler));
    }

    sub_id acquire_id()
    {
        return _next_id++;
    }

    template <class E>
    void publish_sync(const E& event)
    {
        const void* e = static_cast<const void*>(&event);
        std::unique_lock<std::mutex> lock{ _mutex };
        auto[begin, end] = _map.equal_range(get_type<E>());
        std::for_each(begin, end, [=](const auto& entry) { entry.second.handler(e); });
    }

    template <class E>
    void publish_async(const E& event)
    {
        _queue.push([this, event]() { publish_sync(event); });
    }

    template <class E>
    void publish(const E& event)
    {
        publish_sync(event);
    }

    void unsubscribe(sub_id id)
    {
        std::unique_lock<std::mutex> lock{ _mutex };
        erase_if(_map, [=](const auto& entry) { return entry.second.id == id; });
    }

private:
    template <class T>
    static std::type_index get_type()
    {
        return { typeid(T) };
    }

    struct sub_info
    {
        handler_t handler;
        sub_id id;
    };

    std::multimap<std::type_index, sub_info> _map;
    sub_id _next_id;
    task_queue _queue;
    std::mutex _mutex;
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_EVENT_AGGREGATOR_HPP_ */
