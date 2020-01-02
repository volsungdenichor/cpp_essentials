#ifndef CPP_ESSENTIALS_CARTO_COORDS_OPERATIONS_HPP_
#define CPP_ESSENTIALS_CARTO_COORDS_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/carto/coords.hpp>
#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::carto
{

namespace detail
{

struct distance_fn
{
    double operator ()(const coords& from, const coords& to, double radius = 6372.795) const
    {
        using namespace math;

        static const auto haversin = [](double x)
        {
            return sqr(sin(x / 2));
        };

        const auto lat_from = rad(from.latitude);
        const auto lat_to = rad(to.latitude);
        const auto lng_delta = rad(to.longitude - from.longitude);
        const auto a = haversin(lat_to - lat_from) + cos(lat_from) * cos(lat_to) * haversin(lng_delta);
        return a <= 1.0
            ? 2.0 * radius * atan2(sqrt(a), sqrt(1.0 - a))
            : 0.0;
    }
};

struct bearing_fn
{
    double operator ()(const coords& from, const coords& to) const
    {
        using namespace math;

        static const auto normalize = [](double value)
        {
            while (value >= 360.0) value -= 360.0;
            while (value < 0.0) value += 360.0;
            return value;
        };

        const auto lng_delta = rad(to.longitude - from.longitude);
        const auto lat_to = rad(to.latitude);
        const auto lat_from = rad(from.latitude);
        const auto x = cos(lat_from) * sin(lat_to) - sin(lat_from) * cos(lat_to) * cos(lng_delta);
        const auto y = sin(lng_delta) * cos(lat_to);
        const auto bearing = deg(atan2(y, x));

        return normalize(bearing);
    }
};

struct midpoint_fn
{
    coords operator ()(const coords& from, const coords& to) const
    {
        using namespace math;

        const auto lat_to = rad(to.latitude);
        const auto lat_from = rad(from.latitude);
        const auto lng_from = rad(from.longitude);
        const auto lng_delta = rad(to.longitude - from.longitude);

        const auto bx = cos(lat_to) * cos(lng_delta);
        const auto by = cos(lat_to) * sin(lng_delta);
        const auto lat = atan2(sin(lat_from) + sin(lat_to), sqrt(sqr(cos(lat_from) + bx) + sqr(by)));
        const auto lng = lng_from + atan2(by, cos(lat_from) + bx);

        return { deg(lat), deg(lng) };
    }

};

} /* namespace detail */

static constexpr auto distance = detail::distance_fn{};
static constexpr auto bearing = detail::bearing_fn{};
static constexpr auto midpoint = detail::midpoint_fn{};

} /* namespace cpp_essentials::carto */

#endif /* CPP_ESSENTIALS_CARTO_COORDS_OPERATIONS_HPP_ */
