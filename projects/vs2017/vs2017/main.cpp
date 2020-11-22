#include <iostream>
#include <string>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/iterator_range.hpp>
#include <millrind/core/algorithm.hpp>


#define LAMBDA(...) [](auto&& _) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

using namespace millrind::core;

static constexpr auto sqr = [](auto x) { return x * x; };

int main()
{
    transform(range(10),
        std::ostream_iterator<int>{std::cout, " "},
        sqr);
}