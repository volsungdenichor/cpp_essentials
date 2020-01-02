#ifndef CPP_ESSENTIALS_GEO_COORDINATES_CONVERSION_HPP_
#define CPP_ESSENTIALS_GEO_COORDINATES_CONVERSION_HPP_

#pragma once

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct polar_fn
{
    struct from_fn
    {
        template <class T>
        vector_2d<T> operator ()(T radius, T azimuth) const
        {
            return radius * vector_2d<T>({ math::cos(azimuth), math::sin(azimuth) });
        }
    };

    struct radius_fn
    {
        template <class T>
        T operator ()(const vector_2d<T>& value) const
        {
            return length(value);
        }
    };

    struct azimuth_fn
    {
        template <class T>
        T operator ()(const vector_2d<T>& value) const
        {
            return angle(value);
        }
    };

    static constexpr auto from = from_fn{};
    static constexpr auto radius = radius_fn{};
    static constexpr auto azimuth = azimuth_fn{};
};


struct cylindrical_fn
{
    struct from_fn
    {
        template <class T>
        vector_3d<T> operator ()(T radius, T azimuth, T height) const
        {
            return { radius * math::cos(azimuth), height, radius * math::sin(azimuth) };
        }
    };

    struct radius_fn
    {
        template <class T>
        T operator ()(const vector_3d<T>& value) const
        {
            return length(vector_2d<T>({ value[0], value[2] }));
        }
    };

    struct azimuth_fn
    {
        template <class T>
        T operator ()(const vector_3d<T>& value) const
        {
            return angle(vector_2d<T>({ value[0], value[2] }));
        }
    };

    struct height_fn
    {
        template <class T>
        T operator ()(const vector_3d<T>& value) const
        {
            return value[1];
        }
    };

    static constexpr auto from = from_fn{};
    static constexpr auto radius = radius_fn{};
    static constexpr auto azimuth = azimuth_fn{};
    static constexpr auto height = height_fn{};
};


struct spherical_fn
{
    struct from_fn
    {
        template <class T>
        vector_3d<T> operator ()(T radius, T azimuth, T inclination) const
        {
            return radius * vector_3d<T>({
                math::cos(inclination) * math::cos(azimuth),
                math::sin(inclination),
                math::cos(inclination) * math::sin(azimuth) });
        }
    };

    struct radius_fn
    {
        template <class T>
        T operator ()(const vector_3d<T>& value) const
        {
            return length(value);
        }
    };

    struct azimuth_fn
    {
        template <class T>
        T operator ()(const vector_3d<T>& value) const
        {
            return math::atan2(value[2], value[0]);
        }
    };

    struct inclination_fn
    {
        template <class T>
        T operator ()(const vector_3d<T>& value) const
        {
            T r = spherical_radius(value);
            return r ? math::asin(value[1] / r) : math::zero.as<T>();
        }
    };

    static constexpr auto from = from_fn{};
    static constexpr auto radius = radius_fn{};
    static constexpr auto azimuth = azimuth_fn{};
    static constexpr auto inclination = inclination_fn{};
};

} /* namespace detail */

static constexpr auto polar = detail::polar_fn{};
static constexpr auto cylindrical = detail::cylindrical_fn{};

static constexpr auto from_polar = detail::polar_fn::from_fn{};
static constexpr auto polar_radius = detail::polar_fn::radius_fn{};
static constexpr auto polar_azimuth = detail::polar_fn::azimuth_fn{};

static constexpr auto from_cylindrical = detail::cylindrical_fn::from_fn{};
static constexpr auto cylindrical_radius = detail::cylindrical_fn::radius_fn{};
static constexpr auto cylindrical_azimuth = detail::cylindrical_fn::azimuth_fn{};
static constexpr auto cylindrical_height = detail::cylindrical_fn::height_fn{};

static constexpr auto from_spherical = detail::spherical_fn::from_fn{};
static constexpr auto spherical_radius = detail::spherical_fn::radius_fn{};
static constexpr auto spherical_azimuth = detail::spherical_fn::azimuth_fn{};
static constexpr auto spherical_inclination = detail::spherical_fn::inclination_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_COORDINATES_CONVERSION_HPP_ */