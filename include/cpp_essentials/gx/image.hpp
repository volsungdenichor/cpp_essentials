#ifndef CPP_ESSENTIALS_GX_IMAGE_HPP_
#define CPP_ESSENTIALS_GX_IMAGE_HPP_

#pragma once

#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/gx/color_models.hpp>

namespace cpp_essentials::gx
{

template <class T>
struct bytes_per_pixel
{
    static constexpr size_t value = sizeof(T);
};

template <>
struct bytes_per_pixel<byte>
{
    static constexpr size_t value = 1;
};

template <>
struct bytes_per_pixel<rgb_color>
{
    static constexpr size_t value = 3;
};

template <>
struct bytes_per_pixel<rgba_color>
{
    static constexpr size_t value = 4;
};

template <class T>
using image = arrays::array<T, 2, bytes_per_pixel<T>::value>;

using byte_image = image<byte>;
using rgb_image = image<rgb_color>;
using rgba_image = image<rgba_color>;

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
byte_image::view_type channel(ImageView&& img, size_t index, std::false_type)
{
    return { byte_image::pointer(img.data()) + index, img.size(), img.stride() };
}

template <class ImageView>
byte_image::const_view_type channel(ImageView&& img, size_t index, std::true_type)
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
        static constexpr auto channel = channel_fn{};
        return channel(FORWARD(img), Index);
    }
};

} /* namespace detail */

static constexpr auto channel = detail::channel_fn{};
static constexpr auto red_channel = detail::channel_n_fn<0>{};
static constexpr auto green_channel = detail::channel_n_fn<1>{};
static constexpr auto blue_channel = detail::channel_n_fn<2>{};

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_IMAGE_HPP_ */
