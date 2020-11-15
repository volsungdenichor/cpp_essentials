#pragma once

#include <memory>
#include "../iterator_facade.hpp"

namespace core
{

template <class T>
class any_iterator : public iterator_facade<any_iterator<T>>
{
private:
    struct impl
    {
        virtual ~impl() = default;

        virtual std::unique_ptr<impl> clone() const = 0;

        virtual T deref() const = 0;

        virtual void inc() = 0;

        virtual bool is_equal(const impl& other) const = 0;
    };

    template <class Iter>
    struct implementation : impl
    {
        implementation(Iter iter)
            : _iter{ std::move(iter) }
        {
        }

        virtual std::unique_ptr<impl> clone() const override
        {
            return std::make_unique<implementation>(_iter);
        }

        virtual T deref() const override
        {
            return *_iter;
        }

        virtual void inc() override
        {
            ++_iter;
        }

        virtual bool is_equal(const impl& other) const override
        {
            return _iter == static_cast<const implementation&>(other)._iter;
        }

        Iter _iter;
    };

public:
    any_iterator() = default;

    template <class Iter>
    any_iterator(Iter iter)
        : _impl{ std::make_unique<implementation<Iter>>(std::move(iter)) }
    {
    }

    any_iterator(const any_iterator& other)
        : _impl{ other._impl ? other._impl->clone() : nullptr }
    {
    }

    any_iterator(any_iterator&&) = default;

    T deref() const
    {
        return _impl->deref();
    }

    void inc()
    {
        _impl->inc();
    }

    bool is_equal(const any_iterator& other) const
    {
        return (!_impl && !other._impl) || _impl->is_equal(*other._impl);
    }

private:
    std::unique_ptr<impl> _impl;
};

} // namespace core

CORE_ITERATOR_TRAIRS(::core::any_iterator)
