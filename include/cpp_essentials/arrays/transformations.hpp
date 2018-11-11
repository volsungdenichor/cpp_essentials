#ifndef CPP_ESSENTIALS_ARRAYS_TRANSFORMATIONS_HPP_
#define CPP_ESSENTIALS_ARRAYS_TRANSFORMATIONS_HPP_

#include <cpp_essentials/arrays/array_view.hpp>

namespace cpp_essentials::arrays
{


namespace detail
{

template <class T>
typename array_view<T, 2>::location_type get_corner(array_view<T, 2> view, int index)
{
    const auto bounds = view.bounds();

    switch (index)
    {
        case 0:
            return{ bounds[0].min(), bounds[1].min() };
        case 1:
            return{ bounds[0].max(), bounds[1].min() };
        case 2:
            return{ bounds[0].max(), bounds[1].max() };
        case 3:
            return{ bounds[0].min(), bounds[1].max() };
    }

    throw std::invalid_argument { "invalid enum value" };
}

struct flip_fn
{
    template <class T>
    auto operator ()(array_view<T, 1> view) const -> array_view<T, 1>
    {
        if (view.empty())
        {
            return array_view<T, 1> {};
        }

        typename array_view<T, 1>::location_type new_origin = { view.bounds()[0].max() };
        typename array_view<T, 1>::stride_type new_stride = { -view.stride()[0] };

        return array_view<T, 1> { view.data(new_origin), view.size(), new_stride };
    }

    template <class T>
    auto operator ()(array_view<T, 2> view, int direction = 0) const -> array_view<T, 2>
    {
        if (view.empty())
        {
            return array_view<T, 2> {};
        }

        typename array_view<T, 2>::location_type new_origin;
        typename array_view<T, 2>::stride_type new_stride;

        switch (direction)
        {
            case 0:
                new_origin = detail::get_corner(view, 1);
                new_stride = { stride_value_type(-view.stride().get<0>()), stride_value_type(view.stride().get<1>()) };
                break;

            case 1:
                new_origin = detail::get_corner(view, 3);
                new_stride = { stride_value_type(view.stride().get<0>()), stride_value_type(-view.stride().get<1>()) };
                break;

            default:
                throw std::invalid_argument{ "invalid enum value" };
        }

        return array_view<T, 2> { view.data(new_origin), view.size(), new_stride };
    }
};

struct transpose_fn
{
    template <class T>
    auto transpose(array_view<T, 2> view) -> const array_view<T, 2>
    {
        return view.empty()
            ? array_view<T, 2> {}
            : array_view<T, 2>{ view.data(), { view.size().get<1>(), view.size().get<0>() }, { view.stride().get<1>(), view.stride().get<0>() } };
    }
};

struct rotate_fn
{
    template <class T>
    auto operator ()(array_view<T, 2> view, int direction = 1) const -> array_view<T, 2>
    {
        if (view.empty())
        {
            return array_view<T, 2> {};
        }

        direction = direction >= 0 ? direction % 4 : 4 - (-direction % 4);

        typename array_view<T, 2>::location_type new_origin;
        typename array_view<T, 2>::size_type new_size;
        typename array_view<T, 2>::stride_type new_stride;

        switch (direction)
        {
            case 0:
                new_origin = detail::get_corner(view, 0);
                new_size = view.size();
                new_stride = view.stride();
                break;

            case 1:
                new_origin = detail::get_corner(view, 3);
                new_size = { size_value_type(view.size().get<1>()), size_value_type(view.size().get<0>()) };
                new_stride = { stride_value_type(-view.stride().get<1>()), stride_value_type(view.stride().get<0>()) };
                break;

            case 2:
                new_origin = detail::get_corner(view, 2);
                new_size = view.size();
                new_stride = { stride_value_type(-view.stride().get<0>()), stride_value_type(-view.stride().get<1>()) };
                break;

            case 3:
                new_origin = detail::get_corner(view, 1);
                new_size = { size_value_type(view.size().get<1>()), size_value_type(view.size().get<0>()) };
                new_stride = { stride_value_type(view.stride().get<1>()), stride_value_type(-view.stride().get<0>()) };
                break;

            default:
                throw std::invalid_argument{ "invalid enum value" };
        }

        return array_view<T, 2> { view.data(new_origin), new_size, new_stride };
    }
};

struct diagonal_fn
{
    template <class T>
    auto operator ()(array_view<T, 2> view, int direction = 0) const -> array_view<T, 1>
    {
        typename array_view<T, 1>::size_type new_size = std::min(view.size().get<0>(), view.size().get<1>());

        typename array_view<T, 2>::location_type new_origin;
        typename array_view<T, 1>::stride_type new_stride;

        switch (direction)
        {
            case 0:
                new_origin = detail::get_corner(view, 0);
                new_stride = view.stride().get<0>() + view.stride().get<1>();
                break;

            case 1:
                new_origin = detail::get_corner(view, 1);
                new_stride = -view.stride().get<0>() + view.stride().get<1>();
                break;

            case 2:
                new_origin = detail::get_corner(view, 2);
                new_stride = -view.stride().get<0>() - view.stride().get<1>();
                break;

            case 3:
                new_origin = detail::get_corner(view, 3);
                new_stride = +view.stride().get<0>() - view.stride().get<1>();
                break;

            default:
                throw std::invalid_argument{ "invalid enum value" };
        }

        return array_view<T, 1> { view.data(new_origin), new_size, new_stride };
    }
};

} /* namespace detail */

static constexpr detail::flip_fn flip = {};
static constexpr detail::transpose_fn transpose = {};
static constexpr detail::rotate_fn rotate = {};
static constexpr detail::diagonal_fn diagonal = {};

} /* namespace cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_TRANSFORMATIONS_HPP_ */
