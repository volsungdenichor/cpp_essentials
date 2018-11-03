#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_CREATION_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_CREATION_HPP_

#include <cpp_essentials/math/detail/matrix.base.hpp>

namespace cpp_essentials::math
{

namespace detail
{

struct make_vector_t
{
    template <class T>
    vector<T, 1> operator ()(T x) const
    {
        return { x };
    }

    template <class T>
    vector<T, 2> operator ()(T x, T y) const
    {
        return { x, y };
    }

    template <class T>
    vector<T, 3> operator ()(T x, T y, T z) const
    {
        return { x, y, z };
    }

    template <class T, class U = std::remove_reference_t<T>>
    vector<U, 2> operator ()(const std::pair<T, T>& item) const
    {
        return { std::get<0>(item), std::get<1>(item) };
    }


    template <class T, class U = std::remove_reference_t<T>>
    vector<U, 1> operator ()(const std::tuple<T>& item) const
    {
        return { std::get<0>(item) };
    }

    template <class T, class U = std::remove_reference_t<T>>
    vector<U, 2> operator ()(const std::tuple<T, T>& item) const
    {
        return { std::get<0>(item), std::get<1>(item) };
    }

    template <class T, class U = std::remove_reference_t<T>>
    vector<U, 3> operator ()(const std::tuple<T, T, T>& item) const
    {
        return { std::get<0>(item), std::get<1>(item), std::get<2>(item) };
    }

    template <class T, class U = std::remove_reference_t<T>>
    vector<U, 4> operator ()(const std::tuple<T, T, T, T>& item) const
    {
        return { std::get<0>(item), std::get<1>(item), std::get<2>(item), std::get<3>(item) };
    }
};

struct as_tuple_t
{
    template <class T>
    std::tuple<T> operator ()(const vector<T, 1>& item) const
    {
        return { item[0] };
    }

    template <class T>
    std::tuple<T, T> operator ()(const vector<T, 2>& item) const
    {
        return { item[0], item[1] };
    }

    template <class T>
    std::tuple<T, T, T> operator ()(const vector<T, 3>& item) const
    {
        return { item[0], item[1], item[2] };
    }

    template <class T>
    std::tuple<T, T, T, T> operator ()(const vector<T, 4>& item) const
    {
        return { item[0], item[1], item[2], item[3] };
    }
};

struct identity_t
{
public:
    template <size_t D, class T = double>
    square_matrix<T, D> make() const
    {
        square_matrix<T, D> result;

        for (size_t r = 0; r < result.row_count(); ++r)
        {
            for (size_t c = 0; c < result.col_count(); ++c)
            {
                result(r, c) = r == c ? T(1) : T(0);
            }
        }

        return result;
    }

    template <class T, size_t D>
    operator square_matrix<T, D>() const
    {
        return make<D, T>();
    }
};

template <int Value>
class matrix_builder_t
{
public:
    template <size_t R, size_t C, class T = double>
    matrix<T, R, C> make() const
    {
        matrix<T, R, C> result;
        core::fill(result._data, T(Value));
        return result;
    }

    template <size_t D, class T = double>
    vector<T, D> make() const
    {
        return make<1, D, T>();
    }

    template <class T, size_t R, size_t C>
    operator matrix<T, R, C>() const
    {
        return make<R, C, T>();
    }
};

} /* namespace detail */

static constexpr detail::make_vector_t make_vector = {};
static constexpr detail::as_tuple_t as_tuple = {};

static constexpr detail::identity_t identity = {};
static constexpr detail::matrix_builder_t<0> zeros = {};
static constexpr detail::matrix_builder_t<1> ones = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_CREATION_HPP_ */
