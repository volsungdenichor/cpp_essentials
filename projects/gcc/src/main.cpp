#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include <iomanip>
#include <iterator>
#include <deque>
#include "millrind/iterable.hpp"

#ifdef __GNUG__

#include <cstdlib>
#include <memory>
#include <cxxabi.h>

#endif

template <class Tuple, size_t... I>
void print_tuple(std::ostream& os, const Tuple& tuple, std::index_sequence<I...>)
{
    (..., (std::cout << (I == 0 ? "" : ", ") << std::get<I>(tuple)));
}

namespace std
{
template <class... Args>
std::ostream& operator <<(std::ostream& os, const tuple<Args...>& item)
{
    os << "(";
    ::print_tuple(os, item, std::make_index_sequence<sizeof...(Args)>{});
    os << "}";
    return os;
}

template <class T, class U>
std::ostream& operator <<(std::ostream& os, const pair<T, U>& item)
{
    os << "(";
    ::print_tuple(os, item, std::make_index_sequence<2>{});
    os << "}";
    return os;
}
} // namespace std

std::string demangle(const char* name)
{
    int status = -4;
    std::unique_ptr<char, void (*)(void*)> res{ abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free };
    return (status == 0) ? res.get() : name;
}


#define L(...) [&](auto&& _) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

int func(int x)
{
    std::cout << "invoking(" << x << ")" << std::endl;
    return x * x;
}

template <class T>
void print(T& item)
{
    std::cout << static_cast<void*>(&item) << " " << item << std::endl;
}

template <class T>
void print(T&& item) = delete;

struct to_upper_fn
{
    constexpr char operator ()(char ch) const
    {
        return std::toupper(ch);
    }
};

static constexpr inline auto to_upper = to_upper_fn{};

template <class Func>
auto apply(Func func)
{
    return [=](auto&& arg)
    {
        return std::apply(func, std::forward<decltype(arg)>(arg));
    };
}


void run()
{
    using namespace iterables;

    std::string text = "juz w gruzach leza";

    for (const auto& _ : (from(text)
                          | enumerate(100)
                          | map(apply([](int index, char v) { return std::to_string(index) + " " + v; }))))
    {
        std::cout << _ << std::endl;
    }

}

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
}
