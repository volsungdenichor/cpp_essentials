#ifndef CPP_ESSENTIALS_GEO_DCEL_HPP_
#define CPP_ESSENTIALS_GEO_DCEL_HPP_

#pragma once

#include <cpp_essentials/core/tagged_value.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/geo/vertex_container.hpp>

#include <cpp_essentials/core/any_range.hpp>
#include <cpp_essentials/sq/sq.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct dcel_base
{
    using vertex_id = core::tagged_value<int, struct _vertex_id_tag>;
    using face_id = core::tagged_value<int, struct _face_id_tag>;
    using halfedge_id = core::tagged_value<int, struct _halfedge_id_tag>;

    struct vertex_info
    {
        vertex_id id;
        halfedge_id halfedge_id;

        friend std::ostream& operator <<(std::ostream& os, const vertex_info& item)
        {
            return os << "V[" << item.id << "] he=" << item.halfedge_id;
        }
    };

    struct face_info
    {
        face_id id;
        halfedge_id halfedge_id;

        friend std::ostream& operator <<(std::ostream& os, const face_info& item)
        {
            return os << "F[" << item.id << "] he=" << item.halfedge_id;
        }
    };

    struct halfedge_info
    {
        halfedge_id id;
        vertex_id vertex_from_id;
        halfedge_id twin_halfedge_id;
        halfedge_id next_halfedge_id;
        halfedge_id prev_halfedge_id;
        face_id face_id;

        friend std::ostream& operator <<(std::ostream& os, const halfedge_info& item)
        {
            return os << "HE[" << item.id << "] from_v=" << item.vertex_from_id << " twin_he=" << item.twin_halfedge_id << " " << item.prev_halfedge_id << ">" << item.id << ">" << item.next_halfedge_id << " F=" << item.face_id;
        }
    };
};

template <class T>
struct circ_buffer
{
    core::view<std::vector<T>> _range;
    int _size;

    circ_buffer(core::view<std::vector<T>> range)
        : _range{ range }
        , _size{ (int)_range.size() }
    {
    }

    int size() const
    {
        return _size;
    }

    const T& operator [](int index) const
    {
        while (index < 0)
        {
            index += _size;
        }

        return _range.at(index % _size);
    }
};

template <class T>
class dcel : public dcel_base
{
public:
    using location_type = geo::vector_2d<T>;
    using polygon_type = geo::polygon_2d<T>;
    using segment_type = geo::segment_2d<T>;

    struct vertex;
    struct face;
    struct halfedge;

    struct vertex
    {
        const dcel* _owner;
        vertex_id id;

        const location_type& location() const
        {
            return _owner->get_location(id);
        }

        core::enumerable<halfedge> out_halfedges() const
        {
            core::optional<halfedge> next = halfedge{ _owner, info().halfedge_id };
            const auto first_id = next->id;
            return core::make_rust_range([=]() mutable -> core::optional<halfedge>
            {
                auto current = next;
                next = next.map([](const halfedge& h) { return h.twin_halfedge().next_halfedge(); }).filter([=](const halfedge& h) { return h.id != first_id; });
                return current;
            });
        }

        core::enumerable<halfedge> in_halfedges() const
        {
            core::optional<halfedge> next = halfedge{ _owner, info().halfedge_id };
            const auto first_id = next->id;
            return core::make_rust_range([=]() mutable -> core::optional<halfedge>
            {
                auto current = next;
                next = next.map([](const halfedge& h) { return h.twin_halfedge().next_halfedge(); }).filter([=](const halfedge& h) { return h.id != first_id; });
                return current.map([](const halfedge& h) { return h.twin_halfedge(); });
            });
        }

        core::enumerable<face> incident_faces() const
        {
            core::optional<halfedge> next = halfedge{ _owner, info().halfedge_id };
            const auto first_id = next->id;
            return core::make_rust_range([=]() mutable->core::optional<face>
            {
                auto current = next;
                next = next.map([](const halfedge& h) { return h.twin_halfedge().next_halfedge(); }).filter([=](const halfedge& h) { return h.id != first_id; });
                return current.map([](const halfedge& h) { return h.incident_face(); });
            });
        }

        friend std::ostream& operator <<(std::ostream& os, const vertex& item)
        {
            return os << "V: " << item.id << " " << item.location();
        }

    private:
        const vertex_info& info() const
        {
            return _owner->get_vertex(id);
        }
    };

    struct face
    {
        const dcel* _owner;
        face_id id;

        core::enumerable<halfedge> outer_halfedges() const
        {
            core::optional<halfedge> next = halfedge{ _owner, info().halfedge_id };
            const auto first_id = next->id;
            return core::make_rust_range([=]() mutable -> core::optional<halfedge>
            {
                auto current = next;
                next = next.map([](const halfedge& h) { return h.next_halfedge(); }).filter([=](const halfedge& h) { return h.id != first_id; });
                return current;
            });
        }

        core::enumerable<vertex> outer_vertices() const
        {
            core::optional<halfedge> next = halfedge{ _owner, info().halfedge_id };
            const auto first_id = next->id;
            return core::make_rust_range([=]() mutable -> core::optional<vertex>
            {
                auto current = next;
                next = next.map([](const halfedge& h) { return h.next_halfedge(); }).filter([=](const halfedge& h) { return h.id != first_id; });
                return current.map([](const halfedge& h) { return h.vertex_from(); });
            });
        }

        polygon_type as_polygon() const
        {
            return { outer_vertices() | sq::map(&vertex::location) | sq::to_vector() };
        }

        friend std::ostream& operator <<(std::ostream& os, const face& item)
        {
            return os << "F " << item.id;
        }

    private:
        const face_info& info() const
        {
            return _owner->get_face(id);
        }
    };

    struct halfedge
    {
        const dcel* _owner;
        halfedge_id id;

        core::optional<face> incident_face() const
        {
            const auto& i = info();
            return core::eval_optional(i.face_id != face_id{ -1 }, [&]() { return face{ _owner, i.face_id }; });
        }

        halfedge twin_halfedge() const
        {
            return { _owner, info().twin_halfedge_id };
        }

        halfedge next_halfedge() const
        {
            return { _owner, info().next_halfedge_id };
        }

        halfedge prev_halfedge() const
        {
            return { _owner, info().prev_halfedge_id };
        }

        vertex vertex_from() const
        {
            return { _owner, info().vertex_from_id };
        }

        vertex vertex_to() const
        {
            return { _owner, twin_halfedge().vertex_from().id };
        }

        segment_type as_segment() const
        {
            return { vertex_from().location(), vertex_to().location() };
        }

        friend std::ostream& operator <<(std::ostream& os, const halfedge& item)
        {
            return os << item.info();
        }

    private:
        const halfedge_info& info() const
        {
            return _owner->get_halfedge(id);
        }
    };

    vertex_id add_vertex(const location_type& location)
    {
        vertex_info& v = new_vertex();
        set_location(v.id, location);
        return v.id;
    }

    face_id add_face(const std::vector<vertex_id>& vertices)
    {
        face_info& f = new_face();
        build_face(vertices, &f);
        return f.id;
    }

    void add_boundary(const std::vector<vertex_id>& vertices)
    {
        build_face(vertices, nullptr);
    }

    void divide_face(face_id face, vertex_id vertex)
    {
        auto& f = get_face(face);
        auto vertices = get_face_vertices(face);
        circ_buffer<vertex_id> buffer{ vertices };

        build_face({ buffer[0], buffer[1], vertex }, &f);
        f.halfedge_id = *find_halfedge(buffer[0], buffer[1]);

        for (int i = 1; i < buffer.size(); ++i)
        {
            add_face({ buffer[i + 0], buffer[i + 1], vertex });
        }
    }

    core::enumerable<vertex> vertices() const
    {
        return _vertices | sq::map([this](const auto& info) { return vertex{ this, info.id }; });
    }

    core::enumerable<face> faces() const
    {
        return _faces | sq::map([this](const auto& info) { return face{ this, info.id }; });
    }

    core::enumerable<halfedge> halfedges() const
    {
        return _halfedges | sq::map([this](const auto& info) { return halfedge{ this, info.id }; });
    }

private:
    std::vector<vertex_id> get_face_vertices(face_id face) const
    {
        std::vector<vertex_id> result;
        auto halfedge = get_halfedge(get_face(face).halfedge_id);
        auto first_id = halfedge.id;
        do
        {
            result.push_back(halfedge.vertex_from_id);
            halfedge = get_halfedge(halfedge.next_halfedge_id);
        } while (halfedge.id != first_id);
        return result;
    }

    void build_face(const std::vector<vertex_id>& vertices, face_info* face)
    {
        circ_buffer<vertex_id> buffer{ vertices };

        for (int i = 0; i < buffer.size(); ++i)
        {
            connect(buffer[i + 0], buffer[i + 1]);
        }

        for (int i = 0; i < buffer.size(); ++i)
        {
            auto& h0 = get_halfedge(*find_halfedge(buffer[i + 0], buffer[i + 1]));
            auto& h1 = get_halfedge(*find_halfedge(buffer[i + 1], buffer[i + 2]));

            if (auto& v = get_vertex(buffer[i]); v.halfedge_id == halfedge_id{ -1 })
            {
                v.halfedge_id = h0.id;
            }

            h0.next_halfedge_id = h1.id;
            h1.prev_halfedge_id = h0.id;

            if (face)
            {
                if (i == 0)
                {
                    face->halfedge_id = h0.id;
                }

                h0.face_id = face->id;
            }
        }
    }

    core::optional<halfedge_id> find_halfedge(vertex_id from, vertex_id to)
    {
        return core::map_try_get(_edges, std::pair{ from, to });
    }

    std::pair<halfedge_id, halfedge_id> connect(vertex_id from_vertex, vertex_id to_vertex)
    {
        const auto f = find_halfedge(from_vertex, to_vertex);
        const auto t = find_halfedge(to_vertex, from_vertex);

        if (f && t)
        {
            return { *f, *t };
        }

        auto new_halfedges = add_halfedges(2);

        auto& from_halfedge = new_halfedges.at(0);
        auto& to_halfedge = new_halfedges.at(1);

        from_halfedge.vertex_from_id = from_vertex;
        from_halfedge.twin_halfedge_id = to_halfedge.id;

        to_halfedge.vertex_from_id = to_vertex;
        to_halfedge.twin_halfedge_id = from_halfedge.id;

        _edges.emplace(std::pair{ from_vertex, to_vertex }, from_halfedge.id);
        _edges.emplace(std::pair{ to_vertex, from_vertex }, to_halfedge.id);

        return { from_halfedge.id, to_halfedge.id };
    }

    const vertex_info& get_vertex(vertex_id id) const
    {
        return _vertices.at(id.get());
    }

    vertex_info& get_vertex(vertex_id id)
    {
        return _vertices.at(id.get());
    }

    const face_info& get_face(face_id id) const
    {
        return _faces.at(id.get());
    }

    face_info& get_face(face_id id)
    {
        return _faces.at(id.get());
    }

    const halfedge_info& get_halfedge(halfedge_id id) const
    {
        return _halfedges.at(id.get());
    }

    halfedge_info& get_halfedge(halfedge_id id)
    {
        return _halfedges.at(id.get());
    }

    vertex_info& new_vertex()
    {
        auto id = vertex_id{ (int)_vertices.size() };
        _vertices.push_back({ id, halfedge_id{ -1 } });
        return _vertices.back();
    }

    face_info& new_face()
    {
        auto id = face_id{ (int)_faces.size() };
        _faces.push_back({ id, halfedge_id{ -1 } });
        return _faces.back();
    }

    halfedge_info& new_halfedge()
    {
        auto id = halfedge_id{ (int)_halfedges.size() };
        _halfedges.push_back({ id, vertex_id{ -1 }, halfedge_id{ -1 }, halfedge_id{ -1 }, halfedge_id{ -1 }, face_id{ -1 } });
        return _halfedges.back();
    }

    core::mut_view<std::vector<halfedge_info>> add_halfedges(int count)
    {
        for (int i = 0; i < count; ++i)
        {
            new_halfedge();
        }

        return core::views::take_back(_halfedges, count);
    }

    const location_type& get_location(vertex_id id) const
    {
        return _locations.at(id.get());
    }

    void set_location(vertex_id id, const location_type& location)
    {
        _locations.resize(id.get() + 1);
        _locations.at(id.get()) = location;
    }

    std::vector<vertex_info> _vertices;
    std::vector<location_type> _locations;
    std::vector<face_info> _faces;
    std::vector<halfedge_info> _halfedges;
    std::map<std::pair<vertex_id, vertex_id>, halfedge_id> _edges;
};

} /* namespace detail */

using detail::dcel;

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DCEL_HPP_ */