#ifndef CPP_ESSENTIALS_GX_MORPHOLOGICAL_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GX_MORPHOLOGICAL_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/gx/image.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/container_helpers.hpp>

#include <cpp_essentials/gx/detail/morphological_operations.hpp>

namespace cpp_essentials::gx
{

enum class structuring_element
{
    box,
    ellipse
};


inline byte_mask create_structuring_element(size_type size, structuring_element type)
{
    byte_mask result(size);

    switch (type)
    {
        case structuring_element::box:
            core::fill(result, 255);
            break;

        case structuring_element::ellipse:
            auto center = size / 2;

            result
                | sq::iterate()
                | sq::for_each([&](auto&& it)
                {
                    auto d = it.location().as<float>() - center;
                    auto pos = geo::elementwise_divide(2.F * d, size);

                    *it = geo::norm(pos) < 1.F ? 255 : 0;
                });
            break;
    }

    return result;
}

using detail::convolution_t;

using dilation_convolution_t = convolution_t<detail::dilation_tag, 1>;

using erosion_convolution_t = convolution_t<detail::erosion_tag, 1>;

using percentile_convolution_t = convolution_t<detail::percentile_tag, 1>;

template <size_t D>
using kernel_convolution_t = convolution_t<detail::kernel_tag, D>;


const byte_mask ellipse_3x3 = create_structuring_element({ 3, 3 }, structuring_element::ellipse);
const byte_mask ellipse_5x5 = create_structuring_element({ 5, 5 }, structuring_element::ellipse);

const byte_mask box_3x3 = create_structuring_element({ 3, 3 }, structuring_element::box);
const byte_mask box_5x5 = create_structuring_element({ 5, 5 }, structuring_element::box);



inline auto dilation(byte_mask mask) -> dilation_convolution_t
{
    return { std::move(mask) };
}


inline auto erosion(byte_mask mask) -> erosion_convolution_t
{
    return { std::move(mask) };
}


inline auto percentile(byte_mask mask, int rank) -> percentile_convolution_t
{
    return { std::move(mask), rank };
}

inline auto median(byte_mask mask) -> percentile_convolution_t
{
    return percentile(std::move(mask), 50);
}



template <size_t D, class Tag>
void convolve(byte_image::view_type source, byte_image::mut_view_type dest, const convolution_t<Tag, D>& convolution)
{
    auto kernel_size = convolution.size();

    auto size = detail::get_valid_size(source.size(), kernel_size);

    core::transform(
        source.region_range(kernel_size),
        dest.region({ geo::zeros, size }).begin(),
        convolution);
}

template <size_t D, class Tag>
void convolve(rgb_image::view_type source, rgb_image::mut_view_type dest, const convolution_t<Tag, D>& convolution)
{
    for (size_t i = 0; i < 3; ++i)
    {
        convolve(channel(source, i), channel(dest, i), convolution);
    }
}

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_MORPHOLOGICAL_OPERATIONS_HPP_ */
