#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_BASE_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_BASE_HPP_

#pragma once

#include <array>
#include <initializer_list>
#include <iostream>
#include <type_traits>

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/functors.hpp>

#include <cpp_essentials/core/algorithm.hpp>

#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::math
{

template <class T, size_t R, size_t C>
class matrix
{
public:
    using data_type = std::array<T, R * C>;

    using size_type = size_t;

    using value_type = T;

    using reference = typename data_type::reference;
    using const_reference = typename data_type::const_reference;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    matrix()
    {
        core::fill(_data, value_type {});
    }

    matrix(std::initializer_list<value_type> init)
    {
        initialize(init);
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    matrix(std::initializer_list<U> init)
    {
        initialize(init);
    }

    matrix(std::initializer_list<std::initializer_list<value_type>> init)
    {
        initialize(init);
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    matrix(std::initializer_list<std::initializer_list<U>> init)
    {
        initialize(init);
    }

    matrix(const matrix& other)
    {
        core::copy(other._data, _data.begin());
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    matrix(const matrix<U, R, C>& other)
    {
        initialize(other);
    }

    template <class Iter, CONCEPT = cc::Convertible<cc::iter_val<Iter>, value_type>>
    matrix(const core::iterator_range<Iter> range)
    {
        core::transform(range, _data.begin(), core::cast<value_type>);
    }


    matrix& operator =(matrix other)
    {
        std::swap(_data, other._data);
        return *this;
    }    


    constexpr size_type row_count() const
    {
        return R;
    }

    constexpr size_type col_count() const
    {
        return C;
    }

    constexpr size_type size() const
    {
        return _data.size();
    }


    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    matrix<U, R, C> as() const
    {
        return *this;
    }


    const_reference operator ()(size_type r, size_type c) const
    {
        return (*this)[r * col_count() + c];
    }

    reference operator ()(size_type r, size_type c)
    {
        return (*this)[r * col_count() + c];
    }


    const_reference operator [](size_type index) const
    {
        return _data[index];
    }

    reference operator [](size_type index)
    {
        return _data[index];
    }


    template <size_t Index, size_t Row = R, CONCEPT = std::enable_if_t<(Row == 1 && Index < C)>>
    const_reference get() const
    {
        return std::get<Index>(_data);
    }

    template <size_t Index, size_t Row = R, CONCEPT = std::enable_if_t<(Row == 1 && Index < C)>>
    reference get()
    {
        return std::get<Index>(_data);
    }


    template <size_t Row = R, size_t Col = C, CONCEPT = std::enable_if_t<(Row == 1 && Col >= 1)>>
    const_reference x() const
    {
        return get<0>();
    }

    template <size_t Row = R, size_t Col = C, CONCEPT = std::enable_if_t<(Row == 1 && Col >= 1)>>
    reference x()
    {
        return get<0>();
    }


    template <size_t Row = R, size_t Col = C, CONCEPT = std::enable_if_t<(Row == 1 && Col >= 2)>>
    const_reference y() const
    {
        return get<1>();
    }

    template <size_t Row = R, size_t Col = C, CONCEPT = std::enable_if_t<(Row == 1 && Col >= 2)>>
    reference y()
    {
        return get<1>();
    }


    template <size_t Row = R, size_t Col = C, CONCEPT = std::enable_if_t<(Row == 1 && Col >= 3)>>
    const_reference z() const
    {
        return get<2>();
    }

    template <size_t Row = R, size_t Col = C, CONCEPT = std::enable_if_t<(Row == 1 && Col >= 3)>>
    reference z()
    {
        return get<2>();
    }

    data_type _data;

private:
    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    void initialize(const matrix<U, R, C>& other)
    {
        core::transform(other._data, _data.begin(), core::cast<value_type>);
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    void initialize(std::initializer_list<U> init)
    {
        EXPECTS(init.size() == size());

        core::transform(init, _data.begin(), core::cast<value_type>);
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    void initialize(std::initializer_list<std::initializer_list<U>> init)
    {
        EXPECTS(init.size() == row_count());

        auto dest = _data.begin();

        for (auto it = init.begin(); it != init.end(); ++it, dest += col_count())
        {
            EXPECTS(it->size() == col_count());

            core::transform(*it, dest, core::cast<value_type>);
        }
    }
};

template <class T, size_t D>
using square_matrix = matrix<T, D, D>;

template <class T, size_t D>
using vector = matrix<T, 1, D>;


template <class T>
using square_matrix_2d = square_matrix<T, 3>;

template <class T>
using square_matrix_3d = square_matrix<T, 4>;

template <class T>
using vector_2d = vector<T, 2>;

template <class T>
using vector_3d = vector<T, 3>;


template <class T, size_t R, size_t C>
std::ostream& operator <<(std::ostream& os, const matrix<T, R, C>& item)
{
    os << "[";

    for (size_t r = 0; r < item.row_count(); ++r)
    {
        os << "[";

        for (size_t c = 0; c < item.col_count(); ++c)
        {
            if (c != 0)
            {
                os << " ";
            }

            os << item(r, c);
        }

        os << "]";
    }

    os << "]";

    return os;
}

template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const vector<T, D>& item)
{
    os << "[";

    for (size_t d = 0; d < item.size(); ++d)
    {
        if (d != 0)
        {
            os << " ";
        }

        os << item[d];
    }

    os << "]";

    return os;
}

} /* namespace cpp_essentials::math */

namespace std
{

template <class T, size_t D>
struct tuple_size<::cpp_essentials::math::vector<T, D>> : std::integral_constant<std::size_t, D> {};

template <size_t Index, class T, size_t D>
struct tuple_element<Index, ::cpp_essentials::math::vector<T, D>>
{
    using type = decltype(std::declval<::cpp_essentials::math::vector<T, D>>().template get<Index>());
};

} /* namespace std */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_BASE_HPP_ */
