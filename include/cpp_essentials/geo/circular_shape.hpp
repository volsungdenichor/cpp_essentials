#ifndef CPP_ESSENTIALS_GEO_CIRCULAR_SHAPE_HPP_
#define CPP_ESSENTIALS_GEO_CIRCULAR_SHAPE_HPP_

#pragma once

#include <cpp_essentials/geo/matrix.hpp>

namespace cpp_essentials::geo
{

template <class T, size_t D>
class circular_shape
{
public:
    using vector_type = vector<T, D>;

    circular_shape(const vector_type& center, T radius)
        : _center(center)
        , _radius { radius }
    {
    }

    template <class U>
    circular_shape(const circular_shape<U, D>& other)
        : circular_shape(other.center(), other.radius())
    {
    }

    const vector_type& center() const
    {
        return _center;
    }

    T radius() const
    {
        return _radius;
    }

    vector_type _center;
    T _radius;
};

template <class T>
using circle = circular_shape<T, 2>;

template <class T>
using sphere = circular_shape<T, 3>;

template <class T>
using circle_2d = circle<T>;

template <class T>
std::ostream& operator <<(std::ostream& os, const circle<T>& item)
{
    return os << "circle { center:" << item._center << ", radius:" << item._radius << " }";
}

template <class T>
std::ostream& operator <<(std::ostream& os, const sphere<T>& item)
{
    return os << "sphere { center:" << item._center << ", radius:" << item._radius << " }";
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CIRCULAR_SHAPE_HPP_ */
