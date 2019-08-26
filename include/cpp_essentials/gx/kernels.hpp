#ifndef CPP_ESSENTIALS_GX_KERNELS_HPP_
#define CPP_ESSENTIALS_GX_KERNELS_HPP_

#pragma once

#include <cpp_essentials/sq/sq.hpp>

//#include <stats/distributions.hpp>

#include <cpp_essentials/gx/image.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/gx/morphological_operations.hpp>

namespace cpp_essentials::gx
{

enum class orientation
{
    horizontal = 0,
    vertical = 1,
};

namespace kernels
{

namespace detail
{

inline kernel_type normalized(const kernel_type& img)
{
    auto sum = sq::accumulate(img, 0.F);

    kernel_type result { img.size() };

    core::transform(img, result.begin(), [sum](auto v) { return v / sum; });

    return result;
}

} /* namespace detail */

inline auto sharpen() -> kernel_convolution_t<1>
{
    static const float tab [][3] =
    {
        {  0, -1,  0 },
        { -1, +5, -1 },
        {  0, -1,  0 },
    };

    static const kernel_convolution_t<1> result { kernel_array_type<1> { arrays::from(tab) } };

    return result;
}



inline auto blur() -> kernel_convolution_t<1>
{
    static const float tab [][3] =
    {
        { +1, +2, +1 },
        { +2, +4, +2 },
        { +1, +2, +1 },
    };

    static const kernel_convolution_t<1> result = { { detail::normalized(kernel_type { arrays::from(tab) }) } };

    return result;
}



inline auto emboss() -> kernel_convolution_t<1>
{
    static const float tab [][3] =
    {
        { -2, -1,  0 },
        { -1, +1, +1 },
        {  0, +1, +2 },
    };

    static const kernel_convolution_t<1> result { kernel_array_type<1> { arrays::from(tab) } };

    return result;
}



inline auto edge_detect() -> kernel_convolution_t<1>
{
    static const float tab [][3] =
    {
        { -1, -1, -1 },
        { -1, +8, -1 },
        { -1, -1, -1 },
    };

    static const kernel_convolution_t<1> result { kernel_array_type<1> { arrays::from(tab) } };

    return result;
}



inline auto sobel(orientation dir) -> kernel_convolution_t<1>
{
    static const float tab [][3] =
    {
        { -1, 0, +1 },
        { -2, 0, +2 },
        { -1, 0, +1 },
    };

    return { { arrays::rotate(kernel_type::const_view_type(arrays::from(tab)), int(dir)) } };
}

inline auto sobel() -> kernel_convolution_t<2>
{
    return
    { {
        sobel(orientation::horizontal)._kernel,
        sobel(orientation::vertical)._kernel
    } };
}



inline auto prewitt(orientation dir) -> kernel_convolution_t<1>
{
    static const float tab [][3] =
    {
        { -1, 0, +1 },
        { -1, 0, +1 },
        { -1, 0, +1 },
    };

    return { { arrays::rotate(kernel_type::const_view_type(arrays::from(tab)), int(dir)) } };
}

inline auto prewitt() -> kernel_convolution_t<2>
{
    return
    { {
        prewitt(orientation::horizontal)._kernel,
        prewitt(orientation::vertical)._kernel
    } };
}



inline auto cross(orientation dir) -> kernel_convolution_t<1>
{
    static const float tab [][2] =
    {
        { +1,  0 },
        {  0, -1 },
    };

    return { { arrays::rotate(kernel_type::const_view_type(arrays::from(tab)), int(dir)) } };
}

inline auto cross() -> kernel_convolution_t<2>
{
    return
    { {
        cross(orientation::horizontal)._kernel,
        cross(orientation::vertical)._kernel
    } };
}



inline auto box_blur(size_type size) -> kernel_convolution_t<1>
{
    kernel_type result(size);

    core::fill(result, 1.F);

    return { { detail::normalized(result) } };
}

#if 0
inline auto gaussian_blur(size_type size, float sigma) -> kernel_convolution_t<1>
{
    auto gauss = stats::gaussian(sigma);

    auto center = size.as<int>() / 2;

    gx::kernel_type result(size);

    result
        | sq::iterate()
        | sq::for_each([&](auto&& it) { *it = gauss(it.location() - center); });

    return { { detail::normalized(result) } };
}
#endif

} /* namespace kernels */

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_KERNELS_HPP_ */
