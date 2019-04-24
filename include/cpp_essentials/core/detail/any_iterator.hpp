#ifndef CPP_ESSENTIALS_CORE_DETAIL_ANY_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_ANY_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
class any_iterator
    : public core::iterator_facade
    < any_iterator<T>
    , std::forward_iterator_tag
    , T>
{
public:
    using base_type = core::iterator_facade
        < any_iterator<T>
        , std::forward_iterator_tag
        , T>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    class impl_base
    {
    public:
        virtual ~impl_base() {}
        virtual std::unique_ptr<impl_base> clone() const = 0;
        virtual void inc() = 0;
        virtual reference get() const = 0;
        virtual bool is_equal(const impl_base& other) const = 0;
    };

    template <class Iter>
    class implementation : public impl_base
    {
    public:
        implementation(Iter iter)
            : _iter{ std::move(iter) }
        {
        }

        std::unique_ptr<impl_base> clone() const override
        {
            return std::make_unique<implementation>(_iter);
        }
        
        void inc() override
        {
            ++_iter;
        }
        
        reference get() const override
        {
            return *_iter;
        }

        bool is_equal(const impl_base& other) const override
        {
            return _iter == static_cast<const implementation&>(other)._iter;
        }

        Iter _iter;
    };

    any_iterator() = default;

    template <class Iter>
    any_iterator(Iter iter)
        : _impl{ std::make_unique<implementation<Iter>>(iter) }
    {
    }

    any_iterator(const any_iterator& other)
        : _impl{ other._impl ? other._impl->clone() : nullptr }
    {
    }

    any_iterator(any_iterator&&) = default;

    any_iterator& operator =(any_iterator other)
    {
        std::swap(_impl, other._impl);
        return *this;
    }

    void inc()
    {
        _impl->inc();
    }

    reference ref() const
    {
        return _impl->get();
    }

    bool is_equal(const any_iterator& other) const
    {
        return (!_impl && !other._impl) || _impl->is_equal(*other._impl);
    }

private:
    std::unique_ptr<impl_base> _impl;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_ANY_ITERATOR_HPP_ */
