#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/chunk.hpp>

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/elementwise.hpp>

#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/math/polynomial.hpp>

#include <json.hpp>
#include <cpp_essentials/math/regression.hpp>
#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>

using namespace cpp_essentials;


void run()
{
    arrays::array<int, 2> array{ {3, 3} };

    core::iota(array, 100);

    for (auto&& r : arrays::rotate(array.view()).row_range())
    {
        std::cout << core::delimit(r, " ") << std::endl;
        auto s = arrays::flip(r);
        std::cout << "  " << core::delimit(s, " ") << std::endl;
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
    return 0;
}
