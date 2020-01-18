#ifndef CPP_ESSENTIALS_GX_HISTOGRAM_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GX_HISTOGRAM_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/gx/image.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/container_helpers.hpp>

namespace cpp_essentials::gx
{

namespace detail
{

using histogram_t = std::array<size_t, 256>;

inline histogram_t make_histogram(byte_image::const_view_type image)
{
    histogram_t result;
    core::fill(result, 0);

    for (const auto& v : image)
    {
        ++result[v];
    }

    return result;
}

inline histogram_t accumulate_histogram(const histogram_t& histogram)
{
    histogram_t result;
    core::partial_sum(histogram, result.begin());
    return result;
}

struct equalize_fn
{
    lookup_table operator ()(const histogram_t& histogram) const
    {
        const auto cum_hist = accumulate_histogram(histogram);
        const auto size = cum_hist.back();

        const auto min_level = cum_hist | sq::drop_while(core::equal_to(0)) | sq::size();

        return make_lut([&](int v) { return int(255 * (cum_hist[v] - min_level) / (size - min_level)); });
    }
};

struct stretch_fn
{
    lookup_table operator ()(const histogram_t& histogram) const
    {
        auto min = to_byte(0);
        auto max = to_byte(255);

        while (histogram[min] == 0 && min < 255)
        {
            ++min;
        }

        while (histogram[max] == 0 && max > 0)
        {
            --max;
        }

        return levels_adjustment({ min, max }, { 0, 255 });
    }
};

struct otsu_fn
{
    static byte get_threshold(const histogram_t& histogram)
    {
        const auto cum_hist = accumulate_histogram(histogram);

        const auto total = cum_hist.back();

        size_t sum = 0;

        for (size_t i = 0; i < 256; ++i)
        {
            sum += i * histogram[i];
        }

        size_t sumb = 0;

        std::array<size_t, 2> threshold{ 0, 0 };

        float max = 0.F;

        for (size_t i = 0; i < 256; ++i)
        {
            const auto wb = cum_hist[i];

            if (wb == 0)
            {
                continue;
            }

            const auto wf = total - wb;

            if (wf == 0)
            {
                continue;
            }

            sumb += i * histogram[i];

            const auto mb = (float)sumb / wb;
            const auto mf = (float)(sum - sumb) / wf;

            const auto between = wb * wf * math::sqr(mb - mf);

            if (between >= max)
            {
                threshold[0] = i;

                if (between > max)
                {
                    threshold[1] = i;
                }

                max = between;
            }
        }

        return to_byte(core::lerp(0.5F, threshold[0], threshold[1]));
    }

    lookup_table operator ()(const histogram_t& histogram) const
    {
        return threshold(get_threshold(histogram));
    }
};

template <class Op>
struct histogram_operation_fn
{
    void operator ()(byte_image::const_view_type source, byte_image::view_type dest) const
    {
        static const auto op = Op{};
        core::transform(source, dest.begin(), op(make_histogram(source)));
    }

    void operator ()(byte_image::view_type image) const
    {
        (*this)(image, image);
    }

    void operator ()(rgb_image::const_view_type source, rgb_image::view_type dest) const
    {
        for (size_t i = 0; i < 3; ++i)
        {
            (*this)(gx::channel(source, i), gx::channel(dest, i));
        }
    }

    void operator ()(rgb_image::view_type image) const
    {
        (*this)(image, image);
    }
};

} /* namespace detail */

static constexpr auto equalize = detail::histogram_operation_fn<detail::equalize_fn>{};
static constexpr auto stretch = detail::histogram_operation_fn<detail::stretch_fn>{};
static constexpr auto otsu = detail::histogram_operation_fn<detail::otsu_fn>{};

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_HISTOGRAM_OPERATIONS_HPP_ */
