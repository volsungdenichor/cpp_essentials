#ifndef CPP_ESSENTIALS_PH_PH_HPP_
#define CPP_ESSENTIALS_PH_PH_HPP_

#pragma once

#include <cpp_essentials/ph/quantities.hpp>
#include <cpp_essentials/ph/vectors.hpp>

namespace cpp_essentials::ph
{

namespace quantities
{

using scalar = detail::scalar;

using length = detail::quantity_type<1, 0, 0, 0, 0, 0, 0, 0, 0, 0>;
using location = length;
using mass = detail::quantity_type<0, 1, 0, 0, 0, 0, 0, 0, 0, 0>;
using time = detail::quantity_type<0, 0, 1, 0, 0, 0, 0, 0, 0, 0>;
using coords = detail::quantity_type<0, 0, 0, 1, 0, 0, 0, 0, 0, 0>;
using temperature = detail::quantity_type<0, 0, 0, 0, 1, 0, 0, 0, 0, 0>;
using angle = detail::quantity_type<0, 0, 0, 0, 0, 1, 0, 0, 0, 0>;
using current = detail::quantity_type<0, 0, 0, 0, 0, 0, 1, 0, 0, 0>;
using luminous_intensity = detail::quantity_type<0, 0, 0, 0, 0, 0, 0, 1, 0, 0>;
using solid_angle = detail::quantity_type<0, 0, 0, 0, 0, 0, 0, 0, 1, 0>;
using amount = detail::quantity_type<0, 0, 0, 0, 0, 0, 0, 0, 0, 1>;

using area = detail::pow<length, 2>;
using volume = detail::pow<length, 3>;

using velocity = detail::div<length, time>;
using acceleration = detail::div<velocity, time>;

using momentum = detail::mul<mass, velocity>;
using force = detail::mul<mass, acceleration>;
using energy = detail::mul<force, length>;
using power = detail::div<energy, time>;

using mass_flow_rate = detail::div<mass, time>;

using pressure = detail::div<force, area>;
using frequency = detail::inv<time>;

using charge = detail::mul<current, time>;
using voltage = detail::div<power, current>;

using resistance = detail::mul<voltage, current>;
using conductance = detail::inv<resistance>;

using capacitance = detail::div<charge, voltage>;

using magnetic_flux = detail::mul<voltage, time>;
using magnetic_flux_density = detail::div<magnetic_flux, area>;
using inductance = detail::div<magnetic_flux, current>;

using luminous_flux = detail::mul<luminous_intensity, solid_angle>;
using illuminance = detail::div<luminous_flux, area>;

using inertia = detail::mul<mass, area>;
using angular_velocity = detail::div<angle, time>;
using angular_acceleration = detail::div<angular_velocity, time>;

using angular_momentum = detail::mul<inertia, angular_velocity>;
using torque = detail::mul<inertia, angular_acceleration>;
using angular_energy = detail::mul<torque, angle>;

using resolution = detail::div<coords, length>;

} /* namespace quantities */

namespace detail
{

template <class Quant>
struct as_fn
{
    template <class T>
    using quant = quantity<Quant, T>;

    template <class T, CONCEPT = cc::Arithmetic<T>>
    constexpr auto operator ()(T value) const -> quantity<Quant, T>
    {
        return get(value);
    }

    template <class T, CONCEPT = cc::Arithmetic<T>>
    constexpr auto operator ()(T x, T y) const -> vector_2d<Quant, T>
    {
        return { get(x), get(y) };
    }

    template <class T, CONCEPT = cc::Arithmetic<T>>
    constexpr auto operator ()(T x, T y, T z) const -> vector_3d<Quant, T>
    {
        return { get(x), get(y), get(z) };
    }

    template <class T, size_t R, size_t C>
    constexpr auto operator ()(const geo::matrix<T, R, C>& item) const -> matrix<Quant, T, R, C>
    {
        matrix<Quant, T, R, C> result;
        core::transform(item._data, result._data.begin(), std::cref(*this));
        return result;
    }

    template <class T>
    constexpr auto operator ()(const geo::interval<T>& item) const -> interval<Quant, T>
    {
        return{ get(item.lower()), get(item.upper()) };
    }

    template <class Quant, class T, size_t D>
    constexpr auto operator ()(const geo::bounding_box<T, D>& item) const -> bounding_box<Quant, T, D>
    {
        return{ get(item.lower()), get(item.upper()) };
    }

private:
    template <class T>
    auto get(T value) const -> quantity<Quant, T>
    {
        return quantity<Quant, T>{ value };
    }
};

} /* namespace detail */

template <class Quant>
static constexpr auto as = detail::as_fn<Quant>{};

#define DEFINE_QUANTITY(NAME)                           \
                                                        \
template <class T>                                      \
using NAME = quantity<quantities::NAME, T>;             \
                                                        \
static constexpr auto as_##NAME = as<quantities::NAME>; \
                                                        \
namespace literals                                      \
{                                                       \
                                                        \
auto operator "" _##NAME(double long v)                 \
{                                                       \
    return as_##NAME(double(v));                        \
}                                                       \
                                                        \
auto operator "" _##NAME(unsigned long long v)          \
{                                                       \
    return as_##NAME(double(v));                        \
}                                                       \
                                                        \
} /* namespace literals */                              \

DEFINE_QUANTITY(scalar)

DEFINE_QUANTITY(length)
DEFINE_QUANTITY(location)
DEFINE_QUANTITY(mass)
DEFINE_QUANTITY(time)
DEFINE_QUANTITY(coords)
DEFINE_QUANTITY(temperature)
DEFINE_QUANTITY(angle)
DEFINE_QUANTITY(current)
DEFINE_QUANTITY(luminous_intensity)
DEFINE_QUANTITY(solid_angle)
DEFINE_QUANTITY(amount)

DEFINE_QUANTITY(area)
DEFINE_QUANTITY(volume)

DEFINE_QUANTITY(velocity)
DEFINE_QUANTITY(acceleration)

DEFINE_QUANTITY(momentum)
DEFINE_QUANTITY(force)
DEFINE_QUANTITY(energy)
DEFINE_QUANTITY(power)

DEFINE_QUANTITY(mass_flow_rate)

DEFINE_QUANTITY(pressure)
DEFINE_QUANTITY(frequency)

DEFINE_QUANTITY(charge)
DEFINE_QUANTITY(voltage)

DEFINE_QUANTITY(resistance)
DEFINE_QUANTITY(conductance)

DEFINE_QUANTITY(capacitance)

DEFINE_QUANTITY(magnetic_flux)
DEFINE_QUANTITY(magnetic_flux_density)
DEFINE_QUANTITY(inductance)

DEFINE_QUANTITY(luminous_flux)
DEFINE_QUANTITY(illuminance)

DEFINE_QUANTITY(inertia)
DEFINE_QUANTITY(angular_velocity)
DEFINE_QUANTITY(angular_acceleration)

DEFINE_QUANTITY(angular_momentum)
DEFINE_QUANTITY(torque)
DEFINE_QUANTITY(angular_energy)

#undef DEFINE_QUANTITY


#define DEFINE_VECTOR(NAME)                         \
                                                    \
template <class T>                                  \
using NAME##_2d = vector_2d<quantities::NAME, T>;   \
                                                    \
template <class T>                                  \
using NAME##_3d = vector_3d<quantities::NAME, T>;   \

DEFINE_VECTOR(coords)
DEFINE_VECTOR(location)
DEFINE_VECTOR(length)
DEFINE_VECTOR(velocity)
DEFINE_VECTOR(acceleration)
DEFINE_VECTOR(momentum)
DEFINE_VECTOR(force)
DEFINE_VECTOR(resolution)

#undef DEFINE_VECTOR

} /* namespace cpp_essentials::ph */

#endif /* CPP_ESSENTIALS_PH_PH_HPP_ */

