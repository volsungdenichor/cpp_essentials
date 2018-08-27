#ifndef CPP_ESSENTIALS_CORE_PTR_VECTOR_HPP_
#define CPP_ESSENTIALS_CORE_PTR_VECTOR_HPP_

#include <memory>
#include <vector>
#include <algorithm>

#include <cpp_essentials/core/iterator_facade.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T, class Iter>
class ptr_vector_iterator
    : public core::iterator_facade
    < ptr_vector_iterator<T, Iter>
    , std::random_access_iterator_tag
    , T&>
{
public:
    using base_type = core::iterator_facade
        < ptr_vector_iterator<T, Iter>
        , std::random_access_iterator_tag
        , T&>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    ptr_vector_iterator(Iter iter)
        : _iter { iter }
    {
    }

    template <class U, class OtherIter>
    ptr_vector_iterator(ptr_vector_iterator<U, OtherIter> iter)
        : ptr_vector_iterator(iter._iter)
    {
    }

    reference ref() const
    {
        return *get();
    }

    pointer ptr() const
    {
        return get();
    }

    pointer get() const
    {
        return reinterpret_cast<pointer>(*_iter);
    }

    void inc()
    {
        ++_iter;
    }

    void dec()
    {
        --_iter;
    }

    void advance(difference_type offset)
    {
        _iter += offset;
    }

    bool is_equal(const ptr_vector_iterator& other) const
    {
        return _iter == other._iter;
    }

    bool is_less(const ptr_vector_iterator& other) const
    {
        return _iter < other._iter;
    }

    Iter _iter;
};

} /* namespace detail */

template <class T, class Allocator = std::allocator<void*>, class Deleter = std::default_delete<T>>
class ptr_vector
{
public:
    using value_type = std::remove_const_t<T>;
    using const_reference = const T&;
    using reference = T&;
    using const_pointer = const T*;
    using pointer = T*;
    using const_iterator = detail::ptr_vector_iterator<const T, std::vector<void*>::const_iterator>;
    using iterator = detail::ptr_vector_iterator<T, std::vector<void*>::iterator>;
    using const_reverse_iterator = detail::ptr_vector_iterator<const T, std::vector<void*>::const_reverse_iterator>;
    using reverse_iterator = detail::ptr_vector_iterator<T, std::vector<void*>::reverse_iterator>;
    using size_type = std::size_t;

    using allocator_type = Allocator;
    using deleter_type = Deleter;

    ptr_vector() = default;
    ptr_vector(const ptr_vector&) = delete;

    ptr_vector(ptr_vector&&) = default;

    ~ptr_vector()
    {
        clear();
    }

    ptr_vector& operator =(const ptr_vector& other) = delete;
    ptr_vector& operator =(ptr_vector&& other) = default;

    void reserve(size_type size)
    {
        _vect.reserve(size);
    }

    size_type capacity() const
    {
        return _vect.capacity();
    }

    size_type size() const
    {
        return _vect.size();
    }

    bool empty() const
    {
        return _vect.empty();
    }


    void insert(const_iterator pos, std::unique_ptr<T> item)
    {
        _vect.insert(pos._iter, reinterpret_cast<void*>(item.release()));
    }

    template <class U = T, class... Args>
    void emplace(const_iterator pos, Args&&... args)
    {
        insert(pos, std::make_unique<U>(std::forward<Args>(args)...));
    }

    void push_back(std::unique_ptr<T> item)
    {
        insert(cend(), std::move(item));
    }

    template <class U = T, class... Args>
    void emplace_back(Args&&... args)
    {
        emplace<U>(cend(), std::forward<Args>(args)...);
    }


    void erase(const_iterator first, const_iterator last)
    {
        std::for_each(first._iter, last._iter, [this](const void* ptr)
        {
            _deleter(reinterpret_cast<pointer>(const_cast<void*>(ptr)));
        });

        _vect.erase(first._iter, last._iter);
    }

    void erase(const_iterator pos)
    {
        erase(pos, pos + 1);
    }

    void pop_back()
    {
        erase(cend() - 1, cend());
    }

    void clear()
    {
        erase(cbegin(), cend());
    }


    const_reference operator [](size_type index) const
    {
        return cast(_vect[index]);
    }

    reference operator [](size_type index)
    {
        return cast(_vect[index]);
    }

    const_reference at(size_type index) const
    {
        return cast(_vect.at(index));
    }

    reference at(size_type index)
    {
        return cast(_vect.at(index));
    }

    const_reference front() const
    {
        return cast(_vect.front());
    }

    reference front()
    {
        return cast(_vect.front());
    }

    const_reference back() const
    {
        return cast(_vect.back());
    }

    reference back()
    {
        return cast(_vect.back());
    }


    const_iterator begin() const
    {
        return { _vect.begin() };
    }

    const_iterator end() const
    {
        return { _vect.end() };
    }

    iterator begin()
    {
        return { _vect.begin() };
    }

    iterator end()
    {
        return { _vect.end() };
    }

    const_reverse_iterator rbegin() const
    {
        return { _vect.rbegin() };
    }

    const_reverse_iterator rend() const
    {
        return { _vect.rend() };
    }

    reverse_iterator rbegin()
    {
        return { _vect.rbegin() };
    }

    reverse_iterator rend()
    {
        return { _vect.rend() };
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }

    const_reverse_iterator crbegin() const
    {
        return rbegin();
    }

    const_reverse_iterator crend() const
    {
        return rend();
    }

private:
    const_reference cast(void* item) const
    {
        return *reinterpret_cast<const_pointer>(item);
    }

    reference cast(void* item)
    {
        return *reinterpret_cast<pointer>(item);
    }

    std::vector<void*, allocator_type> _vect;
    deleter_type _deleter;
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_PTR_VECTOR_HPP_ */