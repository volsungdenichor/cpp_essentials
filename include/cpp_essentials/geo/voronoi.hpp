#ifndef CPP_ESSENTIALS_GEO_VORONOI_HPP_
#define CPP_ESSENTIALS_GEO_VORONOI_HPP_

#pragma once

#include <unordered_map>

#include <cpp_essentials/sq/sq.hpp>

#include <cpp_essentials/core/algorithm.hpp>

#include <cpp_essentials/geo/dcel.hpp>
#include <cpp_essentials/geo/triangle.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct voronoi_fn
{
    template <class T>
    dcel<T> operator ()(const dcel<T>& input) const
    {
        static const auto get_center = [](typename dcel<T>::face face) -> vector_2d<T>
        {
            return circumcenter(as_triangle(face));
        };

        dcel<T> result;

        const auto outer = input.outer_halfedges() | sq::map([](auto v) { return v.vertex_from().id; }) | sq::to_vector();

        std::unordered_map<typename dcel<T>::face_id, typename dcel<T>::vertex_id> centers;

        const auto is_outer_vertex = [&](typename dcel<T>::vertex vertex)
        {
            return !core::find(outer, vertex.id).empty();
        };

        const auto is_outer_face = [&](typename dcel<T>::face face) -> bool
        {
            return core::any_of(
                face.outer_halfedges(),
                [&](const typename dcel<T>::halfedge& h) { return is_outer_vertex(h.vertex_from()) && is_outer_vertex(h.vertex_to()); });
        };

        const auto add_face = [&](typename dcel<T>::vertex central_vertex, core::iterable<typename dcel<T>::face> faces)
        {
            std::vector<typename dcel<T>::vertex_id> vertices;
            for (auto face : faces)
            {
                typename dcel<T>::vertex_id v;
                if (auto it = centers.find(face.id); it != centers.end())
                {
                    v = it->second;
                }
                else
                {
                    v = result.add_vertex(get_center(face));
                    centers[face.id] = v;
                }
                vertices.push_back(v);
            }

            if (vertices.size() >= 3)
            {
                core::reverse(vertices);
                result.add_face(vertices);
            }
        };

        for (auto v : input.vertices() | sq::drop_if(is_outer_vertex))
        {
            add_face(v, v.incident_faces());
        }

        result.add_boundary();

        return result;
    }

    static triangle_2d<float> as_triangle(const dcel<float>::face& f)
    {
        const auto p = f.as_polygon();
        return { p._data.at(0), p._data.at(1), p._data.at(2) };
    }
};

} /* namespace detail */

static constexpr auto voronoi = detail::voronoi_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_VORONOI_HPP_ */