#pragma once

#include <type_traits>
#include <optional>
#include <functional>
#include <memory>
#include <sstream>

namespace iterables
{

template <class T>
class opt;

template <class T>
class opt : public std::optional<T>
{
public:
    using std::optional<T>::optional;
};

template <class T>
class opt<T&>
{
public:
    constexpr opt() = default;

    constexpr opt(std::nullopt_t)
        : opt()
    {
    }

    constexpr opt(T& value)
        : _value{ &value }
    {
    }

    constexpr opt(const opt&) = default;

    constexpr opt(opt&&) = default;

    constexpr opt& operator =(opt other)
    {
        std::swap(_value, other._value);
        return *this;
    }

    constexpr explicit operator bool() const
    {
        return _value;
    }

    constexpr T& operator *() const
    {
        return *check();
    }

    constexpr T* operator ->() const
    {
        return check();
    }

private:
    constexpr T* check() const
    {
        if (!_value)
            throw std::bad_optional_access{};
        return _value;
    }

    T* _value;
};

template <class T>
std::ostream& operator <<(std::ostream& os, const opt<T>& item)
{
    if (item)
        return os << "some(" << *item << ")";
    else
        return os << "none";
}

template <class T>
struct proxy_type
{
    T value;

    T* operator ->() const
    {
        return &value;
    }
};

template <class T>
class iterable
{
public:
    using opt_type = opt<T>;
    using next_fn = std::function<opt_type()>;

    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using reference = T;
        using value_type = std::remove_reference_t<std::remove_const_t<T>>;
        using pointer = std::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, proxy_type<T>>;
        using difference_type = std::ptrdiff_t;

        constexpr iterator() = default;

        constexpr iterator(next_fn next)
            : _next{ std::move(next) }
            , _current{ _next() }
        {
        }

        constexpr iterator(const iterator&) = default;

        constexpr iterator(iterator&&) = default;

        constexpr reference operator *() const
        {
            return *_current;
        }

        constexpr reference operator ->() const
        {
            if constexpr(std::is_reference_v<T>)
            {
                return pointer{ &**this };
            }
            else
            {
                return pointer{ **this };
            }
        }

        constexpr iterator& operator ++()
        {
            _current = _next();
            return *this;
        }

        constexpr iterator operator ++(int)
        {
            iterator temp{ *this };
            ++(*this);
            return temp;
        }

        friend constexpr bool operator ==(const iterator& lhs, const iterator& rhs)
        {
            return !static_cast<bool>(lhs._current) && !static_cast<bool>(rhs._current);
        }

        friend constexpr bool operator !=(const iterator& lhs, const iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        next_fn _next;
        opt<T> _current;
    };

    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;

    constexpr iterable(next_fn next)
        : _next{ std::move(next) }
    {
    }

    constexpr iterator begin() const
    {
        return { _next };
    }

    constexpr iterator end() const
    {
        return {};
    }

    opt_type next()
    {
        return _next();
    }

    template <class Container, class = std::enable_if_t<std::is_constructible_v<Container, iterator, iterator>>>
    operator Container() const
    {
        return { begin(), end() };
    }


private:
    next_fn _next;
};

template <class T>
using iterator_t = decltype(std::begin(std::declval<T>()));

template <class T>
using iterator_ref = typename std::iterator_traits<T>::reference;

template <class T>
using iterator_val = typename std::iterator_traits<T>::value_type;

template <class T>
using is_iterator = iterator_ref<T>;

template <class T>
using is_range = is_iterator<iterator_t<T>>;

template <class Func>
struct adaptor
{
    Func func;

    template <class T>
    constexpr decltype(auto) operator ()(T&& item) const&
    {
        return std::invoke(func, std::forward<T>(item));
    }

    template <class T>
    constexpr decltype(auto) operator ()(T&& item)&&
    {
        return std::invoke(std::move(func), std::forward<T>(item));
    }
};

template <class Func>
constexpr auto make_adaptor(Func func) -> adaptor<Func>
{
    return { std::move(func) };
}

template <class T, class Func>
constexpr decltype(auto) operator |(T&& item, const adaptor<Func>& adaptor)
{
    return adaptor(std::forward<T>(item));
}

template <class T, class Func>
constexpr decltype(auto) operator |(T&& item, adaptor <Func>&& adaptor)
{
    return (std::move(adaptor))(std::forward<T>(item));
}

template <class L, class R>
constexpr decltype(auto) operator |(adaptor<L> lhs, adaptor <R> rhs)
{
    return make_adaptor([=](auto&& item) { return rhs(lhs(item)); });
}

template <class Parent>
struct with_fn
{
    template <class... Args>
    struct impl
    {
        std::tuple<Args...> args;

        template <class T>
        constexpr auto operator ()(iterable<T> self) const
        {
            return (*this)(std::move(self), std::make_index_sequence<sizeof...(Args)>{});
        }

    private:
        template <class T, size_t... Idx>
        constexpr auto operator ()(iterable<T> self, std::index_sequence<Idx...>) const
        {
            constexpr auto parent = Parent{};
            return parent(std::move(self), std::get<Idx>(args)...);
        }
    };

    template <class... Args>
    constexpr auto operator ()(Args... args) const
    {
        return make_adaptor(impl<Args...>{ std::move(args)... });
    }
};

struct from_fn
{
    template <class Iter, class = is_iterator<Iter>>
    constexpr auto operator ()(Iter begin, Iter end) const
    {
        using ref = iterator_ref<Iter>;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            if (begin != end)
                return opt<ref>{ *begin++ };
            else
                return opt<ref>{};
        } };
    }

    template <class Range, class = is_range<Range>>
    constexpr auto operator ()(Range&& range) const
    {
        return (*this)(std::begin(std::forward<Range>(range)), std::end(std::forward<Range>(range)));
    }

    template <class T>
    constexpr auto operator ()(iterable<T> iterable) const
    {
        return iterable;
    }
};

struct owned_fn
{
    template <class Container>
    constexpr auto operator ()(Container container) const
    {
        using ref = iterator_val<iterator_t<Container>>;

        auto c = std::make_shared<Container>(std::move(container));

        return iterable<ref>{ [c, begin = std::begin(*c), end = std::end(*c)]() mutable -> opt<ref>
        {
            if (begin != end)
                return opt<ref>{ *begin++ };
            else
                return opt<ref>{};
        } };
    }
};

struct container_fn
{
    template <class T, class... Args>
    constexpr auto operator ()(T arg, Args... args) const
    {
        constexpr auto owned = owned_fn{};
        return owned(std::vector<T>{ std::move(arg), std::move(args)... });
    }
};

struct range_fn
{
    template <class T>
    constexpr auto operator ()(T from, T to) const -> iterable<T>
    {
        using ref = T;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            if (from < to)
                return from++;
            else
                return std::nullopt;
        } };
    }

    template <class T>
    constexpr auto operator ()(T to) const -> iterable<T>
    {
        return (*this)(T{}, to);
    }
};

struct infinite_range_fn
{
    template <class T = int>
    constexpr auto operator ()(T from = {}) const -> iterable<T>
    {
        using ref = T;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            return from++;
        } };
    }
};

struct map_fn
{
    template <class T, class Func>
    constexpr auto operator ()(iterable<T> self, Func func) const
    {
        using ref = decltype(std::invoke(func, *self.next()));

        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            if (auto n = self.next())
                return std::invoke(func, *n);
            else
                return std::nullopt;
        } };
    }
};

struct filter_fn
{
    template <class T, class Pred>
    constexpr auto operator ()(iterable<T> self, Pred pred) const
    {
        using ref = T;

        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            while (true)
            {
                if (auto n = self.next(); !n || std::invoke(pred, *n))
                {
                    return n;
                }
            }
        } };
    }
};

struct take_while_fn
{
    template <class T, class Pred>
    constexpr auto operator ()(iterable<T> self, Pred pred) const
    {
        using ref = T;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            if (auto n = self.next(); std::invoke(pred, *n))
            {
                return n;
            }
            return std::nullopt;
        } };
    }
};

struct drop_while_fn
{
    template <class T, class Pred>
    constexpr auto operator ()(iterable<T> self, Pred pred) const
    {
        using ref = T;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            while (auto n = self.next())
            {
                if (!std::invoke(pred, *n))
                {
                    return n;
                }
            }

            return self.next();
        } };
    }

};

struct take_fn
{
    template <class T>
    constexpr auto operator ()(iterable<T> self, std::ptrdiff_t count) const
    {
        using ref = T;
        return iterable<ref>{ [self, count = count]() mutable -> opt<ref>
        {
            if (count-- > 0)
            {
                return self.next();
            }

            return std::nullopt;
        } };
    }
};

struct drop_fn
{
    template <class T>
    constexpr auto operator ()(iterable<T> self, std::ptrdiff_t count) const
    {
        using ref = T;
        return iterable<ref>{ [self, count = count]() mutable -> opt<ref>
        {
            while (count-- > 0)
            {
                self.next();
            }

            return self.next();
        } };
    }
};

struct step_fn
{
    template <class T>
    constexpr auto operator ()(iterable<T> self, std::ptrdiff_t step) const
    {
        using ref = T;
        std::ptrdiff_t counter = 0;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            while (true)
            {
                if (auto n = self.next(); !n || (counter++ % step) == 0)
                {
                    return n;
                }
            }
        } };
    }
};

struct chain_fn
{
    template <class T, class U>
    constexpr auto operator ()(iterable<T> self, iterable<U> other) const
    {
        using ref = std::common_type_t<T, U>;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            if (auto n = self.next(); n)
            {
                return n;
            }
            else if (auto o = other.next(); o)
            {
                return o;
            }
            return std::nullopt;
        } };
    }
};

template <class T, class U>
struct make_tuple
{
    template <class A, class B>
    constexpr auto operator ()(A&& a, B&& b) const
    {
        return std::tuple<T, U>{ std::forward<A>(a), std::forward<B>(b) };
    }
};

struct zip_fn
{
    template <class T, class U, class Func = make_tuple<T, U>>
    constexpr auto operator ()(iterable<T> self, iterable<U> other, Func func = {}) const
    {
        using ref = decltype(std::invoke(func, *self.next(), *other.next()));
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            auto n = self.next();
            auto o = other.next();
            if (!n || !o)
            {
                return std::nullopt;
            }
            return std::invoke(func, *n, *o);
        } };
    }
};

struct enumerate_fn
{
    template <class T, class I = int>
    constexpr auto operator ()(iterable<T> self, I start = {}) const
    {
        constexpr auto zip = zip_fn{};
        constexpr auto infinite_range = infinite_range_fn{};
        return zip(infinite_range(start), std::move(self));
    }
};

struct flatten_fn
{
    template <class T>
    constexpr auto operator ()(iterable<T> self) const
    {
        constexpr auto from = from_fn{};

        using inner = decltype(from(std::declval<T>()));
        using ref = typename inner::reference;

        opt<inner> inner_iter = {};

        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            while (true)
            {
                if (!inner_iter)
                {
                    auto outer = self.next();
                    if (!outer)
                    {
                        return std::nullopt;
                    }

                    inner_iter = from(*outer);
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
        } };
    }
};

struct flat_map_fn
{
    template <class T, class Func>
    constexpr auto operator ()(iterable<T> self, Func func) const
    {
        constexpr auto flatten = flatten_fn{};
        constexpr auto map = map_fn{};
        return flatten(map(std::move(self), std::move(func)));
    }
};

struct nth_fn
{
    template <class T>
    constexpr auto operator ()(iterable<T> self, std::ptrdiff_t count) const
    {
        constexpr auto dro = drop_fn{};
        return drop(std::move(self), count).next();
    }
};

struct repeat_fn
{
    template <class T>
    constexpr auto operator ()(T item) const
    {
        using ref = T;
        return iterable<ref>{ [=]() mutable -> opt<ref>
        {
            return item;
        } };
    }
};

struct once_fn
{
    template <class T>
    constexpr auto operator ()(T item) const
    {
        return take_fn{}(repeat_fn{}(std::move(item)), 1);
    }
};

template <class T>
struct empty_fn
{
    constexpr auto operator ()() const
    {
        using ref = T;
        return iterable<ref>{ []() mutable -> opt<ref> { return std::nullopt; } };
    }
};


struct for_each_fn
{
    template <class T, class Func>
    constexpr auto operator ()(iterable<T> self, Func func) const
    {
        while (auto n = self.next())
        {
            func(*n);
        }
    }
};

struct copy_fn
{
    template <class T, class Output>
    constexpr auto operator ()(iterable<T> self, Output output) const -> Output
    {
        while (auto n = self.next())
        {
            *output = *n;
            output++;
        }
        return output;
    }
};

struct join_fn
{
    template <class T>
    auto operator ()(iterable<T> self, std::string_view separator) const -> std::string
    {
        std::ostringstream ss;
        if (auto n = self.next())
        {
            ss << *n;
        }

        while (auto n = self.next())
        {
            ss << separator << *n;
        }

        return ss.str();
    }
};


static constexpr inline auto range = range_fn{};
static constexpr inline auto infinite_range = infinite_range_fn{};
static constexpr inline auto from = from_fn{};
static constexpr inline auto owned = owned_fn{};
static constexpr inline auto container = container_fn{};

static constexpr inline auto map = with_fn<map_fn>{};
static constexpr inline auto filter = with_fn<filter_fn>{};

static constexpr inline auto take_while = with_fn<take_while_fn>{};
static constexpr inline auto drop_while = with_fn<drop_while_fn>{};

static constexpr inline auto take = with_fn<take_fn>{};
static constexpr inline auto drop = with_fn<drop_fn>{};

static constexpr inline auto step = with_fn<step_fn>{};

static constexpr inline auto chain = chain_fn{};
static constexpr inline auto chain_with = with_fn<chain_fn>{};

static constexpr inline auto zip = zip_fn{};
static constexpr inline auto zip_with = with_fn<zip_fn>{};

static constexpr inline auto enumerate = with_fn<enumerate_fn>{};

static constexpr inline auto flatten = with_fn<flatten_fn>{};
static constexpr inline auto flat_map = with_fn<flat_map_fn>{};

static constexpr inline auto nth = with_fn<nth_fn>{};

static constexpr inline auto repeat = repeat_fn{};
static constexpr inline auto once = once_fn{};

template <class T>
static constexpr inline auto empty = empty_fn<T>{};

static constexpr inline auto for_each = with_fn<for_each_fn>{};
static constexpr inline auto copy = with_fn<copy_fn>{};
static constexpr inline auto join = with_fn<join_fn>{};

} // namespace iterables