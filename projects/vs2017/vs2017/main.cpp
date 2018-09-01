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
#include <cpp_essentials/core/output.hpp>

#include <cpp_essentials/sq/numeric.hpp>

using namespace cpp_essentials;

int main()
{
    std::vector<int> vect = { 2, 4, 5 };
    core::copy(vect, core::cout());

    return 0;
}

