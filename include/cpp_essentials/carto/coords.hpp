#ifndef CPP_ESSENTIALS_CARTO_COORDS_HPP_
#define CPP_ESSENTIALS_CARTO_COORDS_HPP_

#pragma once

#include <iostream>
#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::carto
{

template <class T>
struct coords_base
{
    using value_type = T;

    value_type latitude;
    value_type longitude;
};

template <class T>
bool operator ==(const coords_base<T>& lhs, const coords_base<T>& rhs)
{
    return std::tie(lhs.latitude, lhs.longitude) == std::tie(rhs.latitude, rhs.longitude);
}

template <class T>
bool operator !=(const coords_base<T>& lhs, const coords_base<T>& rhs)
{
    return !(lhs == rhs);
}


template <class T>
std::ostream& operator <<(std::ostream& os, const coords_base<T>& item)
{
    return os << "[" << item.latitude << ", " << item.longitude << "]";
}

using coords = coords_base<double>;

} /* namespace cpp_essentials::carto */

#endif /* CPP_ESSENTIALS_CARTO_COORDS_HPP_ */