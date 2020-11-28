#pragma once

#include <millrind/core/iterators/any_iterator.hpp>
#include <millrind/core/iterators/map_iterator.hpp>
#include <millrind/core/iterators/filter_iterator.hpp>
#include <millrind/core/iterators/iterate_iterator.hpp>
#include <millrind/core/iterators/chain_iterator.hpp>
#include <millrind/core/iterators/filter_map_iterator.hpp>
#include <millrind/core/iterators/flat_map_iterator.hpp>
#include <millrind/core/iterators/generating_iterator.hpp>
#include <millrind/core/iterators/enumerating_iterator.hpp>
#include <millrind/core/iterators/zip_iterator.hpp>
#include <millrind/core/iterators/repeat_iterator.hpp>
#include <millrind/core/iterators/owning_iterator.hpp>
#include <millrind/core/iterators/numeric_iterator.hpp>
#include <millrind/core/iterators/any_iterator.hpp>

namespace millrind::core
{

template <class Func, class Iter>
auto make_map_iterators(Func func, Iter begin, Iter end)
{
    using result_type = map_iterator<Func, Iter>;
    return std::pair{
        result_type{ func, std::move(begin) },
            result_type{ func, std::move(end) }
    };
}

template <class Pred, class Iter>
auto make_filter_iterators(Pred pred, Iter begin, Iter end)
{
    using result_type = filter_iterator<Pred, Iter>;
    return std::pair{
        result_type{ pred, begin, end },
            result_type{ pred, end, end }
    };
}

template <class Iter>
auto make_iterate_iterators(Iter begin, Iter end)
{
    using result_type = iterate_iterator<Iter>;
    return std::pair{
        result_type{ std::move(begin) },
            result_type{ std::move(end) }
    };
}

template <class Func, class Iter>
auto make_flat_map_iterators(Func func, Iter begin, Iter end)
{
    using result_type = flat_map_iterator<Func, Iter>;
    return std::pair{
        result_type{ func, begin, end },
            result_type{ func, end, end }
    };
}

template <class Func, class Iter>
auto make_filter_map_iterators(Func func, Iter begin, Iter end)
{
    using result_type = filter_map_iterator<Func, Iter>;
    return std::pair{
        result_type{ func, begin, end },
            result_type{ func, end, end }
    };
}

template <class Iter1, class Iter2>
auto make_chain_iterators(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2)
{
    using result_type = chain_iterator<Iter1, Iter2>;
    return std::pair{
        result_type{ begin1, begin2, end1, end2 },
            result_type{ end1, end2, end1, end2 }
    };
}

template <class Func, class... Ranges>
auto make_zip_iterators(Func func, Ranges&&... ranges)
{
    static const auto make = [&func](auto... args)
    {
        using result_type = zip_iterator<Func, decltype(args)...>;
        return result_type{ func, std::move(args)... };
    };

    return std::pair{
        make(std::begin(ranges)...),
        make(std::end(ranges)...)
    };
}

template <class Container>
auto make_owning_iterators(Container container)
{
    auto ptr = std::make_shared<Container>(std::move(container));
    auto begin = std::begin(*ptr);
    auto end = std::end(*ptr);

    using result_type = owning_iterator<decltype(begin), Container>;

    return std::pair{
        result_type{ std::move(begin), ptr },
            result_type{ std::move(end), ptr }
    };
}

template <class Iter>
auto make_reverse_iterators(Iter begin, Iter end)
{
    using result_type = std::reverse_iterator<Iter>;
    return std::pair{
        result_type{ end },
            result_type{ begin }
    };
}

template <class Iter>
auto make_reverse_iterators(std::reverse_iterator<Iter> begin, std::reverse_iterator<Iter> end)
{
    using result_type = Iter;
    return std::pair{
        result_type{ end.base() },
            result_type{ begin.base() }
    };
}

template <class T>
auto make_numeric_iterators(T lo, T up)
{
    using result_type = numeric_iterator<T>;
    return std::pair{
        result_type{ lo },
            result_type{ up }
    };
}

template <class T>
auto make_repeat_iterators(T value, std::ptrdiff_t count)
{
    using result_type = repeat_iterator<T>;
    return std::pair{
        result_type{ value, 0 },
            result_type{ value, count },
    };
}

template <class Func>
auto make_generating_iterators(Func func)
{
    using result_type = generating_iterator<Func>;
    return std::pair{
        result_type{ std::move(func) },
            result_type{ },
    };
}

template <class Iter>
auto make_enumerating_iterators(Iter begin, Iter end)
{
    using result_type = enumerating_iterator<Iter>;

    if constexpr (is_detected_v<RandomAccessIterator, Iter>)
    {
        return std::pair{
            result_type{ begin, 0 },
                result_type{ end, std::distance(begin, end) } };
    }
    else
    {
        return std::pair{
            result_type{ begin, 0 },
                result_type{ end, 0 } };
    }
}

} // namespace millrind::core
