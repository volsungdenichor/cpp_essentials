#ifndef CPP_ESSENTIALS_GEO_DETAIL_VECTOR_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_VECTOR_OPERATIONS_HPP_

#include <numeric>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/math/arithmetic_functors.hpp>
#include <cpp_essentials/geo/detail/matrix.base.hpp>

namespace cpp_essentials::geo
{

template <class T>
const auto& width(const vector<T, 2>& item)
{
    return item.x();
}

template <class T>
const auto& height(const vector<T, 2>& item)
{
    return item.y();
}



template <class T, class U, size_t D>
auto dot(const vector<T, D>& lhs, const vector<U, D>& rhs) -> cc::Multiply<T, U>
{
    return core::inner_product(lhs, rhs, cc::Multiply<T, U> {});
}

template <class T, size_t D>
auto norm(const vector<T, D>& item)
{
    return dot(item, item);
}

template <class T, size_t D>
auto length(const vector<T, D>& item)
{
    return math::sqrt(norm(item));
}

template <class T, size_t D>
auto& normalize(vector<T, D>& item)
{
    auto len = length(item);

    if (len)
    {
        item /= len;
    }

    return item;
}

template <class T, size_t D>
auto unit(const vector<T, D>& item)
{
    vector<T, D> result { item };
    normalize(result);
    return result;
}

template <class T, class U, size_t D >
auto distance(const vector<T, D>& lhs, const vector<U, D>& rhs)
{
    return length(rhs - lhs);
}

template <class T, class U, size_t D>
auto manhattan(const vector<T, D>& lhs, const vector<U, D>& rhs)
{
    return sq::inner_product(
        lhs,
        rhs,
        std::common_type_t<T, U> {},
        std::plus<>(),
        [](auto lt, auto rt) { return math::abs(lt - rt); });
}

template <class T, class U>
auto cross(const vector_2d<T>& lhs, const vector_2d<U>& rhs) -> cc::Multiply<T, U>
{
    return lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

template <class T, class U>
auto cross(const vector_3d<T>& lhs, const vector_3d<U>& rhs) -> vector_3d<cc::Multiply<T, U>>
{
    return vector_3d<cc::Multiply<T, U>>{ {
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0] }};
}

template <class T, class U>
auto orientation(const vector_2d<T>& point, const vector_2d<U>& start, const vector_2d<U>& end)
{
    return cross(end - start, point - start);
}



template <class T, size_t D, class E>
bool collinear(const vector<T, D>& lhs, const vector<T, D>& rhs, E epsilon)
{
    return core::approx_equal_to(cross(lhs, rhs), math::zero, epsilon);
}

template <class T>
bool same_direction(const vector_2d<T>& lhs, const vector_2d<T>& rhs)
{
    return cross(lhs, rhs) == math::zero && dot(lhs, rhs) > math::zero;
}


template <class T, size_t D>
auto projection(const vector<T, D>& lhs, const vector<T, D>& rhs)
{
    return rhs * (dot(rhs, lhs) / norm(rhs));
}

template <class T, size_t D>
auto rejection(const vector<T, D>& lhs, const vector<T, D>& rhs)
{
    return lhs - projection(lhs, rhs);
}

//template <class T, class U, size_t D, class R>
//auto interpolate(const vector<T, D>& lhs, const vector<U, D>& rhs, R ratio)
//{
//    return core::lerp(ratio, lhs, rhs);
//}



template <class T>
auto perpendicular(const vector_2d<T>& value)
{
    return vector<T, 2> { -value[1], value[0] };
}

template <class T>
auto angle(const vector_2d<T>& lhs, const vector_2d<T>& rhs) -> T
{
    return math::atan2(cross(lhs, rhs), dot(lhs, rhs));
}

template <class T>
auto angle(const vector_2d<T>& value) -> T
{
    static const vector_2d<T> horizontal = { 1, 0 };

    return angle(horizontal, value);
}

template <class T>
auto angle(const vector_3d<T>& lhs, const vector_3d<T>& rhs)
{
    return math::acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
}

template <class T, size_t D>
auto bisector(const vector<T, D>& lhs, const vector<T, D>& rhs)
{
    return unit(lhs) + unit(rhs);
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_VECTOR_OPERATIONS_HPP_ */