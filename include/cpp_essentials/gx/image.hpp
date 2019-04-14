#ifndef CPP_ESSENTIALS_GX_IMAGE_HPP_
#define CPP_ESSENTIALS_GX_IMAGE_HPP_

#pragma once

#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/gx/color_models.hpp>

namespace cpp_essentials::gx
{

using byte_image = arrays::array<byte, 2>;
using rgb_image = arrays::array<rgb_color, 2, 3>;
using rgba_image = arrays::array<rgba_color, 2, 4>;

using kernel_type = arrays::array<float, 2>;

using size_type = geo::vector<arrays::size_value_type, 2>;
using location_type = geo::vector<arrays::location_value_type, 2>;

template <size_t D>
using kernel_array_type = std::array<kernel_type, D>;



using image_stride_t = arrays::array_stride_t<2>;

using image_size_t = arrays::array_size_t<2>;

using image_location_t = arrays::array_location_t<2>;

using image_bounds_t = arrays::array_bounds_t<2>;

using image_region_t = arrays::array_region_t<2>;



namespace detail
{

template <class ImageView>
byte_image::mut_view_type channel(ImageView&& img, size_t index, std::false_type)
{
    return { byte_image::pointer(img.data()) + index, img.size(), img.stride() };
}

template <class ImageView>
byte_image::view_type channel(ImageView&& img, size_t index, std::true_type)
{
    return { byte_image::const_pointer(img.data()) + index, img.size(), img.stride() };
}

struct channel_fn
{
    template <class ImageView>
    auto operator ()(ImageView&& img, size_t index) const
    {
        return detail::channel(img, index, std::is_const<std::remove_reference_t<typename std::remove_reference_t<ImageView>::reference>>{});
    }
};

template <size_t Index>
struct channel_n_fn
{
    template <class ImageView>
    auto operator ()(ImageView&& img) const
    {
        static constexpr channel_fn channel = {};
        return channel(std::forward<ImageView>(img), Index);
    }
};

} /* namespace detail */

static constexpr detail::channel_fn channel = {};
static constexpr detail::channel_n_fn<0> red_channel = {};
static constexpr detail::channel_n_fn<1> green_channel = {};
static constexpr detail::channel_n_fn<2> blue_channel = {};

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_IMAGE_HPP_ */
