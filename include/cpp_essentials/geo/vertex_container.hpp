#ifndef CPP_ESSENTIALS_GEO_VERTEX_CONTAINER_HPP_
#define CPP_ESSENTIALS_GEO_VERTEX_CONTAINER_HPP_

#pragma once

#include <cpp_essentials/core/any_range.hpp>
#include <cpp_essentials/core/generator.hpp>
#include <cpp_essentials/geo/detail/vertex_container.traits.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct segment_count_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape) const -> size_t
    {
        return vertex_container_traits<Shape>::segment_count(shape);
    }
};

struct vertex_count_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape) const -> size_t
    {
        return vertex_container_traits<Shape>::vertex_count(shape);
    }
};

struct get_segment_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape, size_t index) const -> segment_type<Shape>
    {
        return vertex_container_traits<Shape>::get_segment(shape, index);
    }
};

struct get_vertex_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape, size_t index) const -> vertex_type<Shape>
    {
        return vertex_container_traits<Shape>::get_vertex(shape, index);
    }
};

struct get_segments_fn
{
    template <class Shape>
    auto operator ()(Shape& shape) const
    {
        static const auto _get_segment = get_segment_fn{};
        static const auto _segment_count = segment_count_fn{};
        
        size_t i = 0;
        size_t count = _segment_count(shape);
        
        return core::make_generator([=]() mutable -> core::optional<segment_type<Shape>>
        {
            if (i == count)
                return core::none;
            return _get_segment(shape, i++);
        });
    }
};

struct get_vertices_fn
{
    template <class Shape>
    auto operator ()(Shape& shape) const
    {
        static const auto _get_vertex = get_vertex_fn{};
        static const auto _vertex_count = segment_count_fn{};

        size_t i = 0;
        size_t count = _vertex_count(shape);

        return core::make_generator([=]() mutable -> core::optional<vertex_type<Shape>>
        {
            if (i == count)
                return core::none;
            return _get_vertex(shape, i++);
        });
    }
};

} /* namespace detail */

static constexpr auto segment_count = detail::segment_count_fn{};
static constexpr auto vertex_count = detail::vertex_count_fn{};
static constexpr auto get_segment = detail::get_segment_fn{};
static constexpr auto get_vertex = detail::get_vertex_fn{};

static constexpr auto get_segments = detail::get_segments_fn{};
static constexpr auto get_vertices = detail::get_vertices_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_VERTEX_CONTAINER_HPP_ */