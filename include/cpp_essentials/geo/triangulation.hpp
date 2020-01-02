#ifndef CPP_ESSENTIALS_GEO_TRIANGULATION_HPP_
#define CPP_ESSENTIALS_GEO_TRIANGULATION_HPP_

#pragma once

#include <set>
#include <map>
#include <vector>
#include <cpp_essentials/core/container_helpers.hpp>
#include <cpp_essentials/geo/dcel.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/geo/center.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/contains.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct triangulate_fn
{
    template <class T>
    std::set<size_t> get_vertices(const T& item) const
    {
        return core::to_set(item);
    }

    template <class T>
    void remove(std::vector<T>& lhs, const std::vector<T>& rhs) const
    {
        core::erase_if(
            lhs,
            [&](const T& lhs_item)
        {
            const auto lhs_vertices = get_vertices(lhs_item);
            return core::any_of(rhs, [&](const T& rhs_item)
            {
                return lhs_vertices == get_vertices(rhs_item);
            });
        });
    };

    template <class T>
    dcel<T> operator ()(std::vector<vector_2d<T>> vertices) const
    {
        const auto bounds = make_aabb(vertices);

        const auto max_dimension = std::max(width(bounds), height(bounds));

        const auto c = center(bounds);

        const T delta = T(20);

        using triangle_info = std::array<size_t, 3>;
        using edge_info = std::array<size_t, 2>;

        std::vector<triangle_info> triangles;

        const auto s = vertices.size();

        vertices.push_back(c + vector_2d<T> { -delta * max_dimension, -max_dimension });
        vertices.push_back(c + vector_2d<T> { 0, +delta * max_dimension });
        vertices.push_back(c + vector_2d<T> { +delta * max_dimension, -max_dimension });

        const triangle_info super_triangle{ s + 0, s + 1, s + 2 };

        triangles.push_back(super_triangle);

        const auto get_vertex = [&](size_t index)
        {
            return vertices.at(index);
        };

        const auto get_triangle = [&](const triangle_info& t) -> triangle_2d<T>
        {
            return { get_vertex(t[0]), get_vertex(t[1]), get_vertex(t[2]) };
        };

        static const auto collinear = [](const auto& t)
        {
            return core::approx_equal_to(orientation(t[0], t[1], t[2]), math::zero, 0.0);
        };

        for (size_t i = 0; i < vertices.size(); ++i)
        {
            std::vector<triangle_info> invalid_triangles;
            std::vector<edge_info> edges;

            for (const auto& triangle : triangles)
            {
                if (contains(circumcircle(get_triangle(triangle)), get_vertex(i)))
                {
                    invalid_triangles.push_back(triangle);

                    edges.push_back({ triangle.at(0), triangle.at(1) });
                    edges.push_back({ triangle.at(1), triangle.at(2) });
                    edges.push_back({ triangle.at(2), triangle.at(0) });
                }
            }

            remove(triangles, invalid_triangles);

            std::vector<edge_info> invalid_edges;

            for (const edge_info& edge1 : edges)
            {
                const auto v1 = get_vertices(edge1);
                for (const edge_info& edge2 : edges)
                {
                    if (&edge1 != &edge2 && v1 == get_vertices(edge2))
                    {
                        invalid_edges.push_back(edge1);
                    }
                }
            }

            remove(edges, invalid_edges);

            for (const edge_info& edge : edges)
            {
                triangle_info triangle{ edge[0], edge[1], i };

                if (!collinear(get_triangle(triangle)))
                {
                    triangles.push_back(triangle);
                }
            }
        }

        core::erase_if(
            triangles,
            [&](const triangle_info& triangle)
        {
            const auto t = get_triangle(triangle);
            return core::any_of(super_triangle, [&](size_t super_triangle_vertex)
            {
                return contains(t, get_vertex(super_triangle_vertex));
            });
        });

        dcel<T> result;

        std::map<size_t, typename dcel<T>::vertex_id> map;

        for (const triangle_info& triangle : triangles)
        {
            for (size_t v : triangle)
            {
                if (map.find(v) == map.end())
                {
                    map[v] = result.add_vertex(get_vertex(v));
                }
            }
        }

        for (triangle_info& triangle : triangles)
        {
            const auto t = get_triangle(triangle);
            if (cross(t[1] - t[0], t[2] - t[0]) > 0.0)
            {
                core::reverse(triangle);
            }
            result.add_face({ map.at(triangle[0]), map.at(triangle[1]), map.at(triangle[2]) });
        }

        result.add_boundary();
        return result;
    }
};

} /* namespace detail */

static constexpr auto triangulate = detail::triangulate_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_TRIANGULATION_HPP_ */