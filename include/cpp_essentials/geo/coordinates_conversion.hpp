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

    static constexpr from_fn from = {};
    static constexpr radius_fn radius = {};
    static constexpr azimuth_fn azimuth = {};
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

    static constexpr from_fn from = {};
    static constexpr radius_fn radius = {};
    static constexpr azimuth_fn azimuth = {};
    static constexpr height_fn height = {};
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

    static constexpr from_fn from = {};
    static constexpr radius_fn radius = {};
    static constexpr azimuth_fn azimuth = {};
    static constexpr inclination_fn inclination = {};
};

} /* namespace detail */

static constexpr detail::polar_fn polar = {};
static constexpr detail::cylindrical_fn cylindrical = {};

static constexpr detail::polar_fn::from_fn from_polar = {};
static constexpr detail::polar_fn::radius_fn polar_radius = {};
static constexpr detail::polar_fn::azimuth_fn polar_azimuth = {};

static constexpr detail::cylindrical_fn::from_fn from_cylindrical = {};
static constexpr detail::cylindrical_fn::radius_fn cylindrical_radius = {};
static constexpr detail::cylindrical_fn::azimuth_fn cylindrical_azimuth = {};
static constexpr detail::cylindrical_fn::height_fn cylindrical_height = {};

static constexpr detail::spherical_fn::from_fn from_spherical = {};
static constexpr detail::spherical_fn::radius_fn spherical_radius = {};
static constexpr detail::spherical_fn::azimuth_fn spherical_azimuth = {};
static constexpr detail::spherical_fn::inclination_fn spherical_inclination = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_COORDINATES_CONVERSION_HPP_ */