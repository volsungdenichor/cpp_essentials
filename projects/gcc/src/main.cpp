#include <iostream>
#include <vector>

#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/serialization.hpp>

#include <cpp_essentials/sq/map.hpp>
#include <cpp_essentials/sq/filter.hpp>

using namespace cpp_essentials;

int main()
{
    std::vector<std::string> vect = { "2", "4", "8", "7", "x10", "5" };

    std::cout << (vect | core::all_of([](auto&& x) { return (x | core::size()) < 10; })) << std::endl;

    return 0;
}
