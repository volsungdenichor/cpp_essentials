#ifndef CPP_ESSENTIALS_CORE_CONTAINER_HELPERS_HPP_
#define CPP_ESSENTIALS_CORE_CONTAINER_HELPERS_HPP_

#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/views/slice.hpp>
#include <cpp_essentials/core/serialization.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct insert_fn
{
    template
        < class Container
        , class Range
        , class = cc::InputRange<Range>>
    Container& operator ()(Container& container, cc::range_iter<Container> iter, Range&& range) const
    {
        container.insert(iter, std::begin(range), std::end(range));
        return container;
    }

    template
        < class Container
        , class Range
        , class = cc::InputRange<Range>>
    Container& operator ()(Container& container, Range&& range) const
    {
        container.insert(std::begin(range), std::end(range));
        return container;
    }
};

struct push_front_fn
{
    template
        < class Container
        , class Range
        , class = cc::InputRange<Range>>
    Container& operator ()(Container& container, Range&& range) const
    {
        static constexpr auto _insert = insert_fn{};
        return _insert(container, std::begin(container), FORWARD(range));
    }
};

struct push_back_fn
{
    template
        < class Container
        , class Range
        , class = cc::InputRange<Range>>
    Container& operator ()(Container& container, Range&& range) const
    {
        static constexpr auto _insert = insert_fn{};
        return _insert(container, std::end(container), FORWARD(range));
    }
};

struct erase_fn
{
    template
        < class Container
        , class Range
        , class = cc::InputRange<Range>>
    void operator ()(Container& container, Range&& range) const
    {
        container.erase(std::begin(range), std::end(range));
    }
};

struct erase_if_fn
{
    template
        < class Container
        , class Pred
        , class = cc::UnaryPredicate<Pred, cc::range_ref<Container>>>
    void operator ()(Container& container, Pred&& pred) const
    {
        if constexpr (is_associative_container_v<Container>)
        {
            auto b = std::begin(container);
            auto e = std::end(container);
            while (b != e)
            {
                if (pred(*b))
                {
                    container.erase(b++);
                }
                else
                {
                    ++b;
                }
            }
        }
        else
        {
            static constexpr auto _erase = erase_fn{};
            _erase(container, remove_if(container, std::move(pred)));
        }
    }
};

} /* namespace detail */

static const auto insert = detail::insert_fn{};
static const auto push_front = detail::push_front_fn{};
static const auto push_back = detail::push_back_fn{};
static const auto erase = detail::erase_fn{};
static const auto erase_if = detail::erase_if_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CONTAINER_HELPERS_HPP_ */
