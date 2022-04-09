#ifndef CPP_ESSENTIALS_ASYNC_TASK_QUEUE_HPP_
#define CPP_ESSENTIALS_ASYNC_TASK_QUEUE_HPP_

#pragma once

#include <atomic>
#include <functional>
#include <thread>

#include <cpp_essentials/core/function_defs.hpp>
#include <cpp_essentials/async/channel.hpp>

namespace cpp_essentials::async
{

class task_queue
{
public:
    using task_t = core::action<>;

    task_queue(std::chrono::milliseconds timeout = std::chrono::milliseconds{ 10 })
        : _running{ true }
        , _thread{ std::bind(&task_queue::thread_entry, this, timeout) }
    {
    }

    task_queue(const task_queue&) = delete;
    task_queue(task_queue&&) = delete;

    ~task_queue()
    {
        _running = false;
        _thread.join();
    }

    task_queue& operator =(const task_queue&) = delete;
    task_queue& operator =(task_queue&&) = delete;

    void push(task_t task)
    {
        _channel.push(std::move(task));
    }

    bool empty() const
    {
        return _channel.empty();
    }

private:
    void flush()
    {
        task_t task;
        while (true)
        {
            if (_channel.try_pop(task))
                task();
            else
                break;
        }
    }

    void thread_entry(std::chrono::milliseconds timeout)
    {
        while (true)
        {
            flush();

            std::this_thread::sleep_for(timeout);

            if (!_running && _channel.empty())
                break;
        }
    }

    channel<task_t> _channel;
    std::thread _thread;
    std::atomic<bool> _running;
};

} /* namespace cpp_essentials::async */

#endif /* CPP_ESSENTIALS_ASYNC_TASK_QUEUE_HPP_ */