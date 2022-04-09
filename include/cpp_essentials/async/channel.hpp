#ifndef CPP_ESSENTIALS_ASYNC_CHANNEL_HPP_
#define CPP_ESSENTIALS_ASYNC_CHANNEL_HPP_

#pragma once

#include <condition_variable>
#include <list>
#include <thread>
#include <mutex>

namespace cpp_essentials::async
{

template <class T, class Queue>
class basic_channel
{
public:
    using queue_type = Queue;

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
    {
    }    

    queue_type _queue;
    mutable std::mutex _mutex;
    std::condition_variable _condition_var;   
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

        base_type::_condition_var.wait(lock, std::bind(&ichannel::item_available, this));

        T item = std::move(base_type::_queue.front());
        base_type::_queue.pop_front();
        return item;
    }

    template <class R, class P>
    T pop(const std::chrono::duration<R, P>& timeout)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

        base_type::_condition_var.wait_for(lock, timeout, std::bind(&ichannel::item_available, this));

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

        if (!base_type::_condition_var.wait_for(lock, timeout, std::bind(&ichannel::item_available, this)))
        {
            return false;
        }

        return do_pop(item);
    }

private:
    bool item_available() const
    {
        return !base_type::_queue.empty();
    }

    bool do_pop(T& item)
    {
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

        base_type::_queue.push_back(std::move(item));
        base_type::_condition_var.notify_one();
    }

    bool try_push(T item)
    {
        std::unique_lock<std::mutex> lock{ base_type::_mutex };

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

template <class T, class Queue = std::list<T>>
using channel = iochannel<T, Queue>;

} /* namespace cpp_essentials::async */

#endif /* CPP_ESSENTIALS_ASYNC_CHANNEL_HPP_ */