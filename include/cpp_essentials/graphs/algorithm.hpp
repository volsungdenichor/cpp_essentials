#ifndef CPP_ESSENTIALS_GRAPHS_ALGORITHM_HPP_
#define CPP_ESSENTIALS_GRAPHS_ALGORITHM_HPP_

#pragma once

#include <list>
#include <map>
#include <cpp_essentials/graphs/graph.hpp>

namespace cpp_essentials::graphs
{

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

struct shortest_path_fn
{
    template
        < class Graph
        , class Func
        , class HeuristicFunc
        , class OutputIter
        , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
        , CONCEPT = cc::OutputIterator<OutputIter>>
    OutputIter operator ()(
        const Graph& graph,
        typename Graph::vertex begin,
        typename Graph::vertex end,
        Func&& func,
        HeuristicFunc&& heuristic_func,
        OutputIter output) const
    {
        auto result = shortest_path_impl(graph, begin, end, func, heuristic_func);
        return core::copy(result.vertices, output);
    }

    template
        < class Graph
        , class HeuristicFunc
        , class OutputIter
        , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
        , CONCEPT = cc::OutputIterator<OutputIter>>
    OutputIter operator ()(
        const Graph& graph,
        typename Graph::vertex begin,
        typename Graph::vertex end,
        HeuristicFunc&& heuristic_func,
        OutputIter output) const
    {
        return (*this)(graph, begin, end, [](auto&& e) -> decltype(auto) { return *e; }, heuristic_func, output);
    }

    template
        < class Graph
        , class OutputIter
        , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
        , CONCEPT = cc::OutputIterator<OutputIter>>
    OutputIter operator ()(
        const Graph& graph,
        typename Graph::vertex begin,
        typename Graph::vertex end,
        OutputIter output) const
    {
        return (*this)(graph, begin, end, [](auto&&, auto&&) { return 0; }, output);
    }
};

struct breadth_first_fn
{
    template
        < class Graph
        , class OutputIter
        , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
        , CONCEPT = cc::OutputIterator<OutputIter>>
    OutputIter operator ()(
        const Graph& graph,
        typename Graph::vertex begin,
        OutputIter output) const
    {
        using vertex = typename Graph::vertex;

        std::list<vertex> explored;
        std::list<vertex> queue;

        queue.push_back(begin);

        while (!queue.empty())
        {
            auto v = queue.front();
            queue.pop_front();

            if (!core::find(explored, v))
            {
                *output++ = v;
                explored.push_back(v);
            }

            graph.template vertices<edge_direction::out>(v)
                | sq::drop_if([&](auto u) { return core::find(explored, u); })
                | sq::copy(std::back_inserter(queue));
        }

        return output;
    }
};

template <class Graph, class OutputIter>
void depth_first_impl(
    const Graph& graph,
    std::list<typename Graph::vertex>& explored,
    typename Graph::vertex begin,
    OutputIter output)
{
    if (core::find(explored, begin))
    {
        return;
    }

    *output++ = begin;
    explored.push_back(begin);

    for (auto v : graph.template vertices<edge_direction::out>(begin))
    {
        depth_first_impl(graph, explored, v, output);
    }
}

struct depth_first_fn
{
    template
        < class Graph
        , class OutputIter
        , CONCEPT = std::enable_if_t<detail::is_directed_graph<Graph>::value>
        , CONCEPT = cc::OutputIterator<OutputIter>>
    OutputIter operator ()(
        const Graph& graph,
        typename Graph::vertex begin,
        OutputIter output) const
    {
        using vertex = typename Graph::vertex;

        std::list<vertex> explored;

        depth_first_impl(graph, explored, begin, output);

        return output;
    }
};

} /* namespace detail */

static constexpr detail::shortest_path_fn shortest_path = {};
static constexpr detail::breadth_first_fn breadth_first = {};
static constexpr detail::depth_first_fn depth_first = {};

} /* namespace cpp_essentials::graphs */

#endif /* CPP_ESSENTIALS_GRAPHS_ALGORITHM_HPP_  */
