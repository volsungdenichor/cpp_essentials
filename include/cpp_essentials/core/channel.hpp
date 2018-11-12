#ifndef CPP_ESSENTIALS_CORE_CHANNEL_HPP_
#define CPP_ESSENTIALS_CORE_CHANNEL_HPP_

#pragma once

#include <condition_variable>
#include <list>
#include <thread>
#include <mutex>

namespace cpp_essentials::core
{

template <class T, class Queue>
class basic_channel
{
public:
    using queue_type = Queue;

    void close()
    {
        std::unique_lock<std::mutex> lock{ _mutex };
        _open = false;
        _condition_var.notify_all();
    }

    bool is_open() const
    {
        std::unique_lock<std::mutex> lock{ _mutex };
        return _open;
    }

    bool empty() const
    {
        std::unique_lock<std::mutex> lock{ _mutex };
        return _queue.empty();
    }

protected:
    basic_channel()
        : _queue()
        , _mutex{}
        , _condition_var{}
        , _open{ true }
    {
    }

    void check_if_open() const
    {
        if (!_open)
        {
            throw std::logic_error{ "channel is closed" };
        }
    }

    queue_type _queue;
    mutable std::mutex _mutex;
    std::condition_variable _condition_var;
    bool _open;
};

template <class T, class Queue = std::list<T>>
class ichannel : public virtual basic_channel<T, Queue>
{
private:
    using base_type = basic_channel<T, Queue>;

public:
    ichannel() = default;

    ichannel(const ichannel&) = delete;
    ichannel& operator =(const ichannel&) = delete;

    T pop()
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        base_type::check_if_open();

        base_type::_condition_var.wait(lock, [this]() { return !base_type::_open || !base_type::_queue.empty(); });

        T item = std::move(base_type::_queue.front());
        base_type::_queue.pop_front();
        return item;
    }

    template <class R, class P>
    T pop(const std::chrono::duration<R, P>& timeout)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        base_type::check_if_open();

        base_type::_condition_var.wait_for(lock, timeout, [this]() { return !base_type::_open || !base_type::_queue.empty(); });

        T item = std::move(base_type::_queue.front());
        base_type::_queue.pop_front();
        return item;
    }

    bool try_pop(T& item)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        return do_pop(item);
    }

    template <class R, class P>
    bool try_pop(T& item, const std::chrono::duration<R, P>& timeout)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        if (!base_type::_condition_var.wait_for(lock, timeout, [this]() { return !base_type::_open || !base_type::_queue.empty(); }))
        {
            return false;
        }

        return do_pop(item);
    }

private:
    bool do_pop(T& item)
    {
        if (!base_type::_open)
        {
            return false;
        }

        if (base_type::_queue.empty())
        {
            return false;
        }

        item = std::move(base_type::_queue.front());
        base_type::_queue.pop_front();
        return true;
    }
};

template <class T, class Queue = std::list<T>>
class ochannel : public virtual basic_channel<T, Queue>
{
private:
    using base_type = basic_channel<T, Queue>;

public:
    ochannel() = default;

    ochannel(const ochannel&) = delete;
    ochannel& operator =(const ochannel&) = delete;

    void push(T item)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        base_type::check_if_open();

        base_type::_queue.push_back(std::move(item));
        base_type::_condition_var.notify_one();
    }

    bool try_push(T item)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        if (!base_type::_open)
        {
            return false;
        }

        base_type::_queue.push_back(std::move(item));
        base_type::_condition_var.notify_one();

        return true;
    }
};

template <class T, class Queue = std::list<T>>
class iochannel
    : public ichannel<T, Queue>
    , public ochannel<T, Queue>
{
public:
    iochannel() = default;

    iochannel(const iochannel&) = delete;
    iochannel& operator =(const iochannel&) = delete;
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CHANNEL_HPP_ */