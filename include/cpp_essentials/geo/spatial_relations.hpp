#ifndef CPP_ESSENTIALS_GEO_SPATIAL_RELATIONS_HPP_
#define CPP_ESSENTIALS_GEO_SPATIAL_RELATIONS_HPP_

#include <cpp_essentials/geo/interval.hpp>
#include <cpp_essentials/core/functors.hpp>

namespace cpp_essentials::geo
{

struct contains_t
{
    template <class T, class U, CONCEPT = cc::Convertible<U, T>>
    bool operator ()(const interval<T>& item, U value) const
    {
        return core::between(value, item.lower(), item.upper());
    }

    template <class T, class U>
    bool operator ()(const interval<T>& item, const interval<U>& other) const
    {
        return core::all_of(other._data, core::inclusive_between(item.lower(), item.upper()));
    }
};

struct intersects_t
{
    template <class T, class U, CONCEPT = cc::Convertible<U, T>>
    bool operator ()(const interval<T>& item, const interval<U>& other) const
    {
        return core::any_of(other._data, core::inclusive_between(item.lower(), item.upper()))
            || core::any_of(item._data, core::inclusive_between(other.lower(), other.upper()));
    }
};

struct intersection_t
{
    template <class T>
    auto operator ()(const interval<T>& lhs, const interval<T>& rhs) const -> core::optional<interval<T>>
    {
        auto result = make_intersection(lhs, rhs);
        return core::make_optional(!result.empty(), result);
    }
};

struct clamp_t
{
    template <class V, class T>
    auto operator ()(const T& value, const interval<T>& interval) const
    {
        return core::clamp(value, interval.lower(), interval.upper());
    }
};

struct interpolate_t
{
    template <class T, class R>
    auto operator ()(const interval<T>& item, R ratio) const
    {
        return core::lerp(ratio, item.lower(), item.upper());
    }
};

static constexpr contains_t contains = {};
static constexpr intersects_t intersects = {};
static constexpr intersection_t intersection = {};
static constexpr clamp_t clamp = {};
static constexpr interpolate_t interpolate = {};

} /* namespace cpp_essentials::geo */ 

#endif /* CPP_ESSENTIALS_GEO_SPATIAL_RELATIONS_HPP_ */