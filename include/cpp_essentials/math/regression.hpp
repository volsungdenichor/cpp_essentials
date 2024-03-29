#ifndef CPP_ESSENTIALS_MATH_REGRESSION_HPP_
#define CPP_ESSENTIALS_MATH_REGRESSION_HPP_

#include <cpp_essentials/core/optional.hpp>

#include <cpp_essentials/math/polynomial.hpp>
#include <cpp_essentials/math/matrix.hpp>
#include <cpp_essentials/math/constants.hpp>
#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::math
{

namespace detail
{

template <size_t N, class T = double>
struct regression_fn
{
    template
        < class Range1
        , class Range2
        , class = cc::InputRange<Range1>
        , class = cc::InputRange<Range2>>
    auto operator()(Range1&& range1, Range2&& range2) const -> core::optional<math::polynomial<T, N>>
    {
        auto end1 = std::end(range1);

        auto it1 = std::begin(range1);
        auto it2 = std::begin(range2);

        std::array<T, 2 * N + 1> sum_x;
        std::array<T, N + 1> sum_xy;

        core::fill(sum_x, zero);
        core::fill(sum_xy, zero);

        for (; it1 != end1; ++it1, ++it2)
        {
            auto x = *it1;
            auto y = *it2;

            for (size_t i = 0; i < sum_x.size(); ++i)
            {
                sum_x[i] += pow(x, i);
            }

            for (size_t i = 0; i < sum_xy.size(); ++i)
            {
                sum_xy[i] += pow(x, i) * y;
            }
        }

        square_matrix<T, N + 1> a;

        for (size_t r = 0; r < a.row_count(); ++r)
        {
            for (size_t c = 0; c < a.col_count(); ++c)
            {
                a(r, c) = sum_x[c + r];
            }
        }

        return invert(a).map([&](auto a_inv)
        {
            matrix<T, N + 1, 1> b;
            for (size_t i = 0; i < b.size(); ++i)
            {
                b[i] = sum_xy[i];
            }

            polynomial<T, N> result;
            core::copy((a_inv * b)._data, result._data.begin());
            return result;
        });
    }
};

} /* namespace detail */

template <size_t N, class T = double>
static constexpr auto regression = detail::regression_fn<N, T>{};

static constexpr auto linear_regression = detail::regression_fn<1>{};
static constexpr auto quadratic_regression = detail::regression_fn<2>{};
static constexpr auto cubic_regression = detail::regression_fn<3>{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_REGRESSION_HPP_ */