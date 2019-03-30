#ifndef CPP_ESSENTIALS_CORE_GRAPH_HPP_
#define CPP_ESSENTIALS_CORE_GRAPH_HPP_

#pragma once

#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <list>
#include <climits>

#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/container_helpers.hpp>

namespace cpp_essentials::core
{

enum class edge_direction
{
    in = 1,
    out = 2,
    any = in | out,
};

namespace detail
{

using id_type = int;

template <class V>
struct vertex_info
{
    using vertex_value_type = V;

    vertex_info(id_type id, vertex_value_type value)
        : id{ id }
        , value{ std::move(value) }
    {
    }

    id_type id;
    vertex_value_type value;
};

template <class E, class V>
struct edge_info
{
    using edge_value_type = E;
    using vertex_info = vertex_info<V>;

    edge_info(id_type id, vertex_info& tail, vertex_info& head, edge_value_type value)
        : id{ id }
        , tail{ &tail }
        , head{ &head }
        , value{ std::move(value) }
    {
    }

    id_type id;
    vertex_info* tail;
    vertex_info* head;
    edge_value_type value;
};

template <class V>
class vertex
{
public:
    using vertex_info = vertex_info<V>;
    using vertex_value_type = typename vertex_info::vertex_value_type;

    vertex()
        : _info{ nullptr }
    {
    }

    vertex(const vertex_info& info)
        : _info{ const_cast<vertex_info*>(&info) }
    {
    }

    const vertex_value_type& value() const
    {
        return _info->value;
    }

    explicit operator bool() const
    {
        return _info;
    }

    const vertex_value_type& operator *() const
    {
        return value();
    }

    const vertex_value_type* operator ->() const
    {
        return &value();
    }

    bool operator ==(const vertex& other) const
    {
        return id() == other.id();
    }

    bool operator !=(const vertex& other) const
    {
        return !(*this == other);
    }

    bool operator <(const vertex& other) const
    {
        return id() < other.id();
    }

    bool operator >(const vertex& other) const
    {
        return other < *this;
    }

    bool operator >=(const vertex& other) const
    {
        return !(*this < other);
    }

    bool operator <=(const vertex& other) const
    {
        return !(*this > other);
    }

    friend std::ostream& operator <<(std::ostream& os, const vertex& item)
    {
        return os << "(" << item.value() << ")";
    }

    id_type id() const
    {
        return _info->id;
    }

    //private:
    vertex_info* _info;
};

template <class E, class V>
class edge
{
public:
    using edge_info = edge_info<E, V>;
    using edge_value_type = typename edge_info::edge_value_type;

    using vertex = vertex<V>;

    edge()
        : _info{ nullptr }
    {
    }

    edge(const edge_info& info)
        : _info{ const_cast<edge_info*>(&info) }
    {
    }

    const edge_value_type& value() const
    {
        return _info->value;
    }

    explicit operator bool() const
    {
        return _info;
    }

    const edge_value_type& operator *() const
    {
        return value();
    }

    const edge_value_type* operator ->() const
    {
        return &value();
    }

    vertex tail() const
    {
        return vertex{ *_info->tail };
    }

    vertex head() const
    {
        return vertex{ *_info->head };
    }

    bool operator ==(const edge& other) const
    {
        return id() == other.id();
    }

    bool operator !=(const edge& other) const
    {
        return !(*this == other);
    }

    bool operator <(const edge& other) const
    {
        return id() < other.id();
    }

    bool operator >(const edge& other) const
    {
        return other < *this;
    }

    bool operator >=(const edge& other) const
    {
        return !(*this < other);
    }

    bool operator <=(const edge& other) const
    {
        return !(*this > other);
    }

    friend std::ostream& operator <<(std::ostream& os, const edge& item)
    {
        return os << item.tail() << "-[" << item.value() << "]->" << item.head();
    }

    id_type id() const
    {
        return _info->id;
    }

    //private:
    edge_info* _info;
};

template <edge_direction EdgeDirection>
struct tag_t
{
};

template <class EdgeInfo>
bool edge_contains(id_type vertex_id, const EdgeInfo& edge_info, tag_t<edge_direction::in>)
{
    return vertex_id == edge_info.head->id;
}

template <class EdgeInfo>
bool edge_contains(id_type vertex_id, const EdgeInfo& edge_info, tag_t<edge_direction::out>)
{
    return vertex_id == edge_info.tail->id;
}

template <class EdgeInfo>
bool edge_contains(id_type vertex_id, const EdgeInfo& edge_info, tag_t<edge_direction::any>)
{
    return vertex_id == edge_info.head->id || vertex_id == edge_info.tail->id;;
}

template <edge_direction EdgeDirection>
struct edge_contains_vertex
{
    const id_type vertex_id;

    template <class MapItem>
    bool operator ()(const MapItem& item) const
    {
        static const tag_t<EdgeDirection> tag = {};

        return edge_contains(vertex_id, *std::get<1>(item), tag);
    }
};

template <class V>
struct get_opposite_vertex
{
    using vertex = V;

    const id_type vertex_id;

    template <class MapItem>
    vertex operator ()(const MapItem& item) const
    {
        const auto& edge_info = *std::get<1>(item);
        const auto& vertex_info = vertex_id != edge_info.head->id
            ? *edge_info.head
            : *edge_info.tail;

        return vertex{ vertex_info };
    }
};

template <class E>
struct get_edge
{
    using edge = E;

    template <class MapItem>
    edge operator ()(const MapItem& item) const
    {
        return edge{ *std::get<1>(item) };
    }
};

template <class V>
struct get_vertex
{
    using vertex = V;

    template <class MapItem>
    vertex operator ()(const MapItem& item) const
    {
        return vertex{ *std::get<1>(item) };
    }
};


template <class V, class VertexMap>
auto get_vertices(const VertexMap& vertices)
{
    return map(vertices, get_vertex<V>{});
}

template <class E, class EdgeMap>
auto get_edges(const EdgeMap& edges)
{
    return map(edges, get_edge<E> {});
}

template <class V, edge_direction EdgeDirection, class EdgeMap>
auto get_vertices(const EdgeMap& edges, id_type id)
{
    return map(take_if(edges, edge_contains_vertex<EdgeDirection> { id }), get_opposite_vertex<V> { id });
}

template <class E, edge_direction EdgeDirection, class EdgeMap>
auto get_edges(const EdgeMap& edges, id_type id)
{
    return map(take_if(edges, edge_contains_vertex<EdgeDirection> { id }), get_edge<E>{});
}


template <class V, class E, bool Owning>
class directed_graph_base
{
public:
    using vertex_value_type = V;
    using edge_value_type = E;

    using vertex = detail::vertex<V>;
    using edge = detail::edge<E, V>;


    void erase(vertex v)
    {
        EXPECTS((bool)v);
        erase_if(_edges, edge_contains_vertex<edge_direction::any> { v.id() });
        _vertices.erase(v.id());
    }

    void erase(edge e)
    {
        EXPECTS((bool)e);
        _edges.erase(e.id());
    }

    void clear()
    {
        _vertices.clear();
        _edges.clear();
    }


    vertex find(const vertex_value_type& value) const
    {
        for (const auto&[k, v] : _vertices)
        {
            if (v->value == value)
            {
                return { *v };
            }
        }
        return {};
    }


    const vertex_value_type& operator [](vertex v) const
    {
        EXPECTS((bool)vertex);
        return _vertices.at(v.id())->value;
    }

    vertex_value_type& operator [](vertex v)
    {
        EXPECTS((bool)vertex);
        return _vertices.at(v.id())->value;
    }


    const edge_value_type& operator [](edge e) const
    {
        EXPECTS((bool)e);
        return _edges.at(e.id())->value;
    }

    edge_value_type& operator [](edge e)
    {
        EXPECTS((bool)e);
        return _edges.at(e.id())->value;
    }


    std::vector<vertex> vertices() const
    {
        return get_vertices<vertex>(_vertices);
    }

    std::vector<edge> edges() const
    {
        return get_edges<edge>(_edges);
    }

    template <edge_direction EdgeDirection = edge_direction::any>
    std::vector<vertex> vertices(vertex v) const
    {
        EXPECTS((bool)v);
        return get_vertices<vertex, EdgeDirection>(_edges, v.id());
    }

    template <edge_direction EdgeDirection = edge_direction::any>
    std::vector<edge> edges(vertex v) const
    {
        EXPECTS((bool)v);
        return get_edges<edge, EdgeDirection>(_edges, v.id());
    }

    template <edge_direction EdgeDirection = edge_direction::any>
    size_t degree(vertex v) const
    {
        EXPECTS((bool)v);
        return size(vertices<EdgeDirection>(v));
    }

    void swap(directed_graph_base& other)
    {
        std::swap(_vertices, other._vertices);
        std::swap(_edges, other._edges);
    }

protected:
    using id_type = detail::id_type;
    using vertex_info = detail::vertex_info<V>;
    using edge_info = detail::edge_info<E, V>;
    using vertex_map_value = std::conditional_t<Owning, std::unique_ptr<vertex_info>, vertex_info*>;
    using edge_map_value = std::conditional_t<Owning, std::unique_ptr<edge_info>, edge_info*>;

    using vertex_map = std::map<id_type, vertex_map_value>;
    using edge_map = std::map<id_type, edge_map_value>;

    vertex_map _vertices;
    edge_map _edges;
};

} /* namespace detail */


template <class V, class E>
class directed_graph_view : public detail::directed_graph_base<V, E, false>
{
public:
    using base_type = detail::directed_graph_base<V, E, false>;

    using vertex_value_type = typename base_type::vertex_value_type;
    using edge_value_type = typename base_type::edge_value_type;
    using vertex = typename base_type::vertex;
    using edge = typename base_type::edge;


    void insert(vertex v)
    {
        base_type::_vertices.emplace(v.id(), v._info);
    }

    void insert(edge e)
    {
        insert(e.head());
        insert(e.tail());

        base_type::_edges.emplace(e.id(), e._info);
    }
};


template <class V, class E>
class directed_graph : public detail::directed_graph_base<V, E, true>
{
public:
    using base_type = detail::directed_graph_base<V, E, true>;

    using vertex_value_type = typename base_type::vertex_value_type;
    using edge_value_type = typename base_type::edge_value_type;
    using vertex = typename base_type::vertex;
    using edge = typename base_type::edge;

    using view_type = directed_graph_view<V, E>;

    directed_graph()
        : base_type()
        , _next_vertex_id{ 0 }
        , _next_edge_id{ 0 }
    {
    }

    directed_graph(directed_graph&& other)
        : directed_graph{}
    {
        swap(other);
    }

    directed_graph(const directed_graph&) = delete;

    ~directed_graph()
    {
        clear();
    }

    directed_graph& operator =(directed_graph other)
    {
        swap(other);
        return *this;
    }


    view_type view() const
    {
        view_type result;

        for (auto v : base_type::vertices())
        {
            result.insert(v);
        }

        for (auto e : base_type::edges())
        {
            result.insert(e);
        }

        return result;
    }


    vertex insert(vertex_value_type vertex_value)
    {
        auto id = next_vertex();
        auto info = std::make_unique<base_type::vertex_info>(id, std::move(vertex_value));
        base_type::_vertices.emplace(id, std::move(info));
        return vertex{ *base_type::_vertices.at(id) };
    }

    vertex find_or_insert(const vertex_value_type& vertex_value)
    {
        auto v = base_type::find(vertex_value);
        return v
            ? v
            : insert(vertex_value);
    }

    edge insert(vertex tail, vertex head, edge_value_type edge_value)
    {
        EXPECTS((bool)tail);
        EXPECTS((bool)head);

        auto id = next_edge();

        auto info = std::make_unique<base_type::edge_info>(id, *base_type::_vertices.at(tail.id()), *base_type::_vertices.at(head.id()), std::move(edge_value));

        base_type::_edges.emplace(id, std::move(info));
        return edge{ *base_type::_edges.at(id) };
    }

    edge insert(const vertex_value_type& tail, const vertex_value_type& head, edge_value_type edge_value)
    {
        return insert(find_or_insert(tail), find_or_insert(head), std::move(edge_value));
    }

    void clear()
    {
        base_type::clear();

        _next_vertex_id = 0;
        _next_edge_id = 0;
    }

    void swap(directed_graph& other)
    {
        std::swap(_next_vertex_id, other._next_vertex_id);
        std::swap(_next_edge_id, other._next_edge_id);
        base_type::swap(other);
    }

private:
    using id_type = typename base_type::id_type;

    id_type next_vertex()
    {
        return _next_vertex_id++;
    }

    id_type next_edge()
    {
        return _next_edge_id++;
    }

    id_type _next_vertex_id;
    id_type _next_edge_id;
};

namespace detail
{

template <class T>
struct is_directed_graph : std::false_type
{
};

template <class V, class E>
struct is_directed_graph<directed_graph<V, E>> : std::true_type
{
};

template <class V, class E>
struct is_directed_graph<directed_graph_view<V, E>> : std::true_type
{
};

template <class V>
std::list<V> revert_path(const std::map<V, V>& prev, V last)
{
    std::list<V> result;

    while (true)
    {
        result.push_front(last);

        auto it = prev.find(last);

        if (it != prev.end())
        {
            last = it->second;
        }
        else
        {
            break;
        }
    }

    return result;
}

template <class V, class D>
std::list<std::pair<V, D>> revert_path(const std::map<V, V>& prev, const std::map<V, D>& dist, V last)
{
    std::list<std::pair<V, D>> result;

    while (true)
    {
        auto d = dist.at(last);
        result.push_front({ last, d });

        auto it = prev.find(last);

        if (it != prev.end())
        {
            last = it->second;
        }
        else
        {
            break;
        }
    }

    return result;
}

template <class V, class D>
struct path_result
{
    std::list<V> vertices;
    std::map<V, D> dist;

    D distance(V vertex) const
    {
        return dist.at(vertex);
    }
};

template <class Graph, class Func, class HeuristicFunc>
auto shortest_path_impl(
    const Graph& graph,
    typename Graph::vertex begin,
    typename Graph::vertex end,
    Func&& func,
    HeuristicFunc&& heuristic_func)
{
    using vertex_type = typename Graph::vertex;
    using edge_type = typename Graph::edge;

    using distance_type = std::decay_t<std::invoke_result_t<Func, edge_type>>;

    using priority_type = std::decay_t<cc::Add<distance_type, std::invoke_result_t<HeuristicFunc, vertex_type, vertex_type>>>;

    static const distance_type infinite_distance = std::numeric_limits<distance_type>::max();
    static const priority_type infinite_priority = std::numeric_limits<priority_type>::max();

    std::map<vertex_type, vertex_type> previous;
    std::map<vertex_type, distance_type> distance;
    std::priority_queue<std::pair<priority_type, vertex_type>> queue;

    distance[begin] = distance_type{};

    queue.push({ priority_type {}, begin });

    while (!queue.empty())
    {
        auto u = queue.top().second;
        queue.pop();

        auto dist_u = distance[u];

        for (auto edge : graph.template edges<edge_direction::out>(u))
        {
            auto v = edge.tail() != u
                ? edge.tail()
                : edge.head();

            auto iter = distance.find(v);

            auto dist = dist_u + func(edge);

            if (iter == distance.end() || dist < iter->second)
            {
                distance[v] = dist;
                previous[v] = u;

                auto priority = priority_type{ dist + heuristic_func(v, end) };
                queue.push({ priority, v });
            }
        }
    }

    auto vertices = revert_path(previous, end);

    return path_result<vertex_type, distance_type> { vertices, distance };
}

} /* namespace detail */

template
    < class Graph
    , class Func
    , class HeuristicFunc
    , class OutputIter
    , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
    , CONCEPT = cc::OutputIterator<OutputIter>>
OutputIter shortest_path(
    const Graph& graph,
    typename Graph::vertex begin,
    typename Graph::vertex end,
    Func&& func,
    HeuristicFunc&& heuristic_func,
    OutputIter output)
{
    auto result = detail::shortest_path_impl(graph, begin, end, func, heuristic_func);
    return copy(result.vertices, output);
}

template
    < class Graph
    , class HeuristicFunc
    , class OutputIter
    , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
    , CONCEPT = cc::OutputIterator<OutputIter>>
OutputIter shortest_path(
    const Graph& graph,
    typename Graph::vertex begin,
    typename Graph::vertex end,
    HeuristicFunc&& heuristic_func,
    OutputIter output)
{
    return shortest_path(graph, begin, end, [](auto&& e) -> decltype(auto) { return *e; }, heuristic_func, output);
}

template
    < class Graph
    , class OutputIter
    , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
    , CONCEPT = cc::OutputIterator<OutputIter>>
OutputIter shortest_path(
    const Graph& graph,
    typename Graph::vertex begin,
    typename Graph::vertex end,
    OutputIter output)
{
    return shortest_path(graph, begin, end, [](auto&&, auto&&) { return 0; }, output);
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_GRAPH_HPP_ */

