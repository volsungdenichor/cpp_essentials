#ifndef CPP_ESSENTIALS_GX_DETAIL_MORPHOLOGICAL_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GX_DETAIL_MORPHOLOGICAL_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/gx/image.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/container_helpers.hpp>

namespace cpp_essentials::gx
{

using byte_mask = byte_image;

namespace detail
{

size_type get_valid_size(size_type image_size, size_type kernel_size)
{
    return image_size - kernel_size + size_type { 1, 1 };
}

template <class Tag, size_t D>
struct convolution_t
{
    byte operator ()(byte_image::const_view_type region) const;
    size_type size() const;
};

struct dilation_tag {};
struct erosion_tag {};
struct percentile_tag {};
struct kernel_tag {};

template <>
struct convolution_t<dilation_tag, 1>
{
    convolution_t(byte_mask mask)
        : _mask(std::move(mask))
    {
    }

    byte operator ()(byte_image::const_view_type region) const
    {
        auto result = core::max_value(core::make_range(region) * core::make_range(_mask));
        return result / 256;
    }

    size_type size() const
    {
        return _mask.size();
    }

    byte_mask _mask;
};



template <>
struct convolution_t<erosion_tag, 1>
{
    convolution_t(byte_mask mask)
        : _mask(std::move(mask))
    {
    }

    byte operator ()(byte_image::const_view_type region) const
    {
        auto result = core::max_value((255 - core::make_range(region)) * core::make_range(_mask));
        return 255 - (result / 256);
    }

    size_type size() const
    {
        return _mask.size();
    }

    byte_mask _mask;
};



template <>
struct convolution_t<percentile_tag, 1>
{
    convolution_t(byte_mask mask, int rank)
        : _mask(std::move(mask))
        , _rank(rank)
    {
    }

    byte operator ()(byte_image::const_view_type region) const
    {
        _vect.clear();
        core::push_back(_vect, core::make_range(region) * core::make_range(_mask) / 256);

        auto index = _vect.size() * _rank / 100;
        std::nth_element(_vect.begin(), _vect.begin() + index, _vect.end());

        return _vect[index];
    }

    size_type size() const
    {
        return _mask.size();
    }

    byte_mask _mask;
    int _rank;
    mutable std::vector<byte> _vect;
};



template <size_t D>
struct convolution_t<kernel_tag, D>
{
    convolution_t(const kernel_array_type<D>& kernels)
    {
        core::move(kernels, _kernels.begin());
    }

    byte operator ()(byte_image::const_view_type region) const
    {
        geo::vector<float, D> vect;

        for (size_t i = 0; i < D; ++i)
        {
            vect[i] = core::inner_product(region, _kernels[i], 0.F);
        }

        auto sum = geo::length(vect);

        return to_byte(sum);
    }

    size_type size() const
    {
        return _kernels[0].size();
    }

    kernel_array_type<D> _kernels;
};

template <>
struct convolution_t<kernel_tag, 1>
{
    convolution_t(const kernel_array_type<1>& kernels)
        : _kernel(std::move(kernels[0]))
    {
    }

    byte operator ()(byte_image::const_view_type region) const
    {
        auto sum = core::inner_product(region, _kernel, 0.F);

        return to_byte(sum);
    }

    size_type size() const
    {
        return _kernel.size();
    }

    kernel_type _kernel;
};

} /* namespace detail */

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_DETAIL_MORPHOLOGICAL_OPERATIONS_HPP_ */
