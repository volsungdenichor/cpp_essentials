#ifndef CPP_ESSENTIALS_GEO_BEZIER_HPP_
#define CPP_ESSENTIALS_GEO_BEZIER_HPP_

#pragma once

#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/math/binomial.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct make_bezier_fn
{
    struct at_fn
    {
        template <class Range, class T>
        auto operator ()(Range&& range, T t) const
        {
            using vertex_type = cc::range_val<Range>;
            using value_type = typename vertex_type::value_type;

            auto n = (int)core::size(range);

            vertex_type result;

            auto first = 0;
            auto last = n - 1;

            for (const auto& v : range)
            {
                result += v * (value_type)(math::pow(math::one - t, last) * math::pow(t, first) * math::binomial(n - 1, first));
                ++first;
                --last;
            }

            return result;
        }
    };

    template <class Range, class OutputIter>
    OutputIter operator ()(Range&& range, OutputIter output, size_t count) const
    {
        static constexpr auto _at = at_fn{};
        
        for (size_t i = 0; i < count; ++i)
        {
            *output++ = _at(range, (float)i / (count - 1));
        }

        return output;
    }

    struct polyline_fn
    {
        template <class Range>
        auto operator ()(Range&& range, size_t count) const
        {
            static constexpr auto _make_bezier = make_bezier_fn{};
            
            using vertex_type = cc::range_val<Range>;
            using value_type = typename vertex_type::value_type;

            std::vector<vertex_type> v;
            _make_bezier(range, std::back_inserter(v), count);

            return polyline<value_type, matrix_traits<vertex_type>::columns>{ std::move(v) };
        }
    };
};

} /* namespace detail */

static constexpr auto bezier_at = detail::make_bezier_fn::at_fn{};
static constexpr auto make_bezier = detail::make_bezier_fn{};
static constexpr auto make_bezier_polyline = detail::make_bezier_fn::polyline_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_BEZIER_HPP_ */

