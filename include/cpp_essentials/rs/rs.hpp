#ifndef CPP_ESSENTIALS_RS_RS_HPP_
#define CPP_ESSENTIALS_RS_RS_HPP_

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/arithmetic_functors.hpp>

#include <vector>
#include <sstream>

#pragma once

namespace cpp_essentials::rs
{

template <class T>
using Option = cpp_essentials::core::optional<T>;

template <class Ref>
using NextFn = std::function<Option<Ref>()>;

template <class Ref>
class Iterator
{
public:
    using Reference = Ref;
    using Item = std::decay_t<Reference>;

    Iterator(NextFn<Reference> next)
        : _next{ std::move(next) }
    {
    }

    Option<Ref> next()
    {
        return _next();
    }

private:
    NextFn<Reference> _next;
};

namespace detail
{

struct iter_fn
{
    template <class Iter, CONCEPT = cc::Iterator<Iter>>
    auto operator ()(Iter begin, Iter end) const
    {
        using ref = typename std::iterator_traits<Iter>::reference;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            return begin != end ? Option<ref>{ *begin++ } : Option<ref>{};
        });
    }

    template <class Container, CONCEPT = cc::Range<Container>>
    auto operator ()(Container&& container) const
    {
        return (*this)(std::begin(container), std::end(container));
    }

    template <class Ref>
    auto operator ()(Iterator<Ref> it) const
    {
        return it;
    }
};

static constexpr auto iter = iter_fn{};


struct range_fn
{
    auto operator ()(int min, int max) const
    {
        using ref = int;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            return min < max ? Option<ref>{ min++ } : Option<ref>{};
        });
    }

    auto operator ()(int max) const
    {
        return (*this)(0, max);
    }
};

static constexpr auto range = range_fn{};


struct infinite_range_fn
{
    auto operator ()(int min) const
    {
        using ref = int;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            return min++;
        });
    }

    auto operator ()() const
    {
        return (*this)(0);
    }
};

static constexpr auto infinite_range = infinite_range_fn{};


struct repeat_fn
{
    template <class T>
    auto operator ()(T item) const
    {
        using ref = T;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            return item;
        });
    }
};

static constexpr auto repeat = repeat_fn{};


struct once_fn
{
    template <class T>
    auto operator ()(T item) const
    {
        int count = 1;
        using ref = T;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            if (count-- > 0)
            {
                return item;
            }

            return core::none;
        });
    }
};

static constexpr auto once = once_fn{};


template <class T>
struct empty_fn
{
    auto operator ()() const
    {
        using ref = T;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            return core::none;
        });
    }
};

template <class T>
static constexpr auto empty = empty_fn<T>{};


struct map_fn
{
    template <class Ref, class Func>
    auto operator ()(Iterator<Ref> self, Func func) const
    {
        using ref = decltype(func(*self.next()));
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            if (auto n = self.next())
            {
                return func(*n);
            }
            return core::none;
        });
    }
};

static constexpr auto map = core::adaptable{ map_fn{} };


struct for_each_fn
{
    template <class Ref, class Func>
    auto operator ()(Iterator<Ref> self, Func func) const
    {
        while (auto n = self.next())
        {
            func(*n);
        }
    }
};

static constexpr auto for_each = core::adaptable{ for_each_fn{} };


struct filter_fn
{
    template <class Ref, class Pred>
    auto operator ()(Iterator<Ref> self, Pred pred) const
    {
        using ref = Ref;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            while (true)
            {
                if (auto n = self.next(); !n || pred(*n))
                {
                    return n;
                }
            }
        });
    }
};

static constexpr auto filter = core::adaptable{ filter_fn{} };


struct partition_fn
{
    template <class Ref, class Pred>
    auto operator ()(Iterator<Ref> self, Pred pred) const
    {
        return core::make_tuple(filter(self, pred), filter(self, core::logical_negation(pred)));
    }
};

static constexpr auto partition = core::adaptable{ partition_fn{} };


struct filter_map_fn
{
    template <class Ref, class Func>
    auto operator ()(Iterator<Ref> self, Func func) const
    {
        using ref = core::underlying_type_t<decltype(func(*self.next()))>;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            while (true)
            {
                if (auto n = self.next())
                {
                    if (auto r = func(*n); r)
                    {
                        return *r;
                    }
                }
                else
                {
                    return core::none;
                }
            }
        });
    }
};

static constexpr auto filter_map = core::adaptable{ filter_map_fn{} };


struct take_while_fn
{
    template <class Ref, class Pred>
    auto operator ()(Iterator<Ref> self, Pred pred) const
    {
        using ref = Ref;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            if (auto n = self.next(); pred(*n))
            {
                return n;
            }
            return core::none;
        });
    }
};

static constexpr auto take_while = core::adaptable{ take_while_fn{} };


struct take_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self, int count) const
    {
        using ref = Ref;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            if (count-- > 0)
            {
                return self.next();
            }

            return core::none;
        });
    }
};

static constexpr auto take = core::adaptable{ take_fn{} };


struct skip_while_fn
{
    template <class Ref, class Pred>
    auto operator ()(Iterator<Ref> self, Pred pred) const
    {
        using ref = Ref;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            while (auto n = self.next())
            {
                if (!pred(*n))
                {
                    return n;
                }
            }

            return self.next();
        });
    }
};

static constexpr auto skip_while = core::adaptable{ skip_while_fn{} };


struct skip_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self, int count) const
    {
        using ref = Ref;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            while (count-- > 0)
            {
                self.next();
            }

            return self.next();
        });
    }
};

static constexpr auto skip = core::adaptable{ skip_fn{} };


struct zip_fn
{
    template <class Ref, class Other, class Func>
    auto operator ()(Iterator<Ref> self, Iterator<Other> other, Func func) const
    {
        using ref = std::invoke_result_t<Func, Ref, Other>;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            auto n = self.next();
            auto o = other.next();
            if (!n || !o)
            {
                return core::none;
            }
            return func(*n, *o);
        });
    }

    template <class Ref, class Other>
    auto operator ()(Iterator<Ref> self, Iterator<Other> other) const
    {
        return (*this)(std::move(self), std::move(other), core::make_tuple);
    }
};

static constexpr auto zip = core::adaptable{ zip_fn{} };


struct enumerate_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self, int min = 0) const
    {
        return infinite_range(min) | zip(self);
    }
};

static constexpr auto enumerate = core::adaptable{ enumerate_fn{} };


struct to_vector_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self) const
    {
        using T = typename Iterator<Ref>::Item;
        std::vector<T> result;
        while (auto n = self.next())
        {
            result.push_back(*n);
        }
        return result;
    }
};

static constexpr auto to_vector = core::adaptable{ to_vector_fn{} };


struct join_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self, std::string_view separator = {}) const
    {
        std::stringstream ss;
        bool first = true;
        while (auto n = self.next())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                ss << separator;
            }
            ss << *n;
        }
        return ss.str();
    }
};

static constexpr auto join = core::adaptable{ join_fn{} };


template <bool C, bool R>
struct check_elements_fn
{
    template <class Ref, class Pred>
    bool operator ()(Iterator<Ref> self, Pred pred) const
    {
        while (auto n = self.next())
        {
            if (pred(*n) == C)
            {
                return R;
            }
        }
        return !R;
    }
};

static constexpr auto all = core::adaptable{ check_elements_fn<false, true>{} };
static constexpr auto any = core::adaptable{ check_elements_fn<true, true>{} };
static constexpr auto none = core::adaptable{ check_elements_fn<true, false>{} };


struct count_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self) const
    {
        size_t result = 0;
        while (auto n = self.next())
        {
            ++result;
        }
        return result;
    }
};

static const auto count = core::adaptable{ count_fn{} };


struct flatten_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self) const
    {
        using Inner = decltype(iter(std::declval<Ref>()));
        using ref = typename Inner::Reference;

        core::optional<Inner> inner_iter = {};

        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            while (true)
            {
                if (!inner_iter)
                {
                    auto outer = self.next();
                    if (!outer)
                    {
                        return core::none;
                    }

                    inner_iter = iter(*outer);
                    continue;
                }

                auto n = inner_iter->next();

                if (n)
                {
                    return *n;
                }
                else
                {
                    inner_iter = {};
                    continue;
                }
            }
        });
    }
};

static constexpr auto flatten = core::adaptable{ flatten_fn{} };


struct flat_map_fn
{
    template <class Ref, class Func>
    auto operator ()(Iterator<Ref> self, Func func) const
    {
        return self | map(func) | flatten();
    }
};

static constexpr auto flat_map = core::adaptable{ flat_map_fn{} };


struct nth_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self, int count) const
    {
        return (self | skip(count)).next();
    }
};

static constexpr auto nth = core::adaptable{ nth_fn{} };


struct last_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self) const
    {
        Option<Ref> result = {};
        while (auto n = self.next())
        {
            result = n;
        }
        return result;
    }
};

static constexpr auto last = core::adaptable{ last_fn{} };


struct chain_fn
{
    template <class Ref, class Other>
    auto operator ()(Iterator<Ref> self, Iterator<Other> other) const
    {
        using ref = std::common_type_t<Ref, Other>;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            if (auto n = self.next(); n)
            {
                return n;
            }
            else if (auto o = other.next(); o)
            {
                return o;
            }
            return core::none;
        });
    }
};

static constexpr auto chain = core::adaptable{ chain_fn{} };


struct step_by_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self, int step) const
    {
        using ref = Ref;
        int counter = 0;
        return Iterator<ref>([=]() mutable -> Option<ref>
        {
            while (true)
            {
                if (auto n = self.next(); !n || (counter++ % step) == 0)
                {
                    return n;
                }
            }
        });
    }
};

static constexpr auto step_by = core::adaptable{ step_by_fn{} };


struct fold_fn
{
    template <class Ref, class T, class Func>
    auto operator ()(Iterator<Ref> self, T init, Func func) const
    {
        while (auto n = self.next())
        {
            init = func(init, *n);
        }
        return init;
    }
};

static constexpr auto fold = core::adaptable{ fold_fn{} };


struct sum_fn
{
    template <class Ref>
    auto operator ()(Iterator<Ref> self) const
    {
        return fold(self, typename Iterator<Ref>::Item{}, core::plus);
    }
};

static constexpr auto sum = core::adaptable{ sum_fn{} };


struct position_fn
{
    template <class Ref, class Pred>
    auto operator ()(Iterator<Ref> self, Pred pred) const
    {
        size_t index = 0;
        while (auto n = self.next())
        {
            if (pred(*n))
            {
                return core::optional<size_t>{ index };
            }
            index++;
        }
        return core::optional<size_t>{};
    }
};

static constexpr auto position = core::adaptable{ position_fn{} };

} /* namespace detail */

using detail::iter;
using detail::range;
using detail::infinite_range;
using detail::once;
using detail::repeat;
using detail::empty;

using detail::join;
using detail::to_vector;

using detail::map;
using detail::for_each;
using detail::filter;
using detail::partition;
using detail::flatten;
using detail::chain;
using detail::enumerate;
using detail::filter_map;
using detail::flat_map;
using detail::last;
using detail::nth;
using detail::skip;
using detail::skip_while;
using detail::take;
using detail::take_while;
using detail::step_by;

using detail::all;
using detail::any;
using detail::none;
using detail::count;
using detail::fold;
using detail::sum;
using detail::position;

} /* namespace cpp_essentials::rs */

#endif /* CPP_ESSENTIALS_RS_RS_HPP_ */
