#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/core/math_functors.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/serialization.hpp>

#include <cpp_essentials/sq/numeric.hpp>

using namespace cpp_essentials;

int main()
{
    std::vector<int> vect = sq::range(5);
    auto p = std::vector<int>{ 3, 4, 5 };
    std::cout << (vect | core::contains(p)) << std::endl;

    return 0;
}

