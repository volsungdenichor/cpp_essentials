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
#include <cpp_essentials/math/matrix.hpp>
#include <cpp_essentials/geo/matrix.hpp>

using namespace cpp_essentials;


void run()
{
    geo::square_matrix_2d<float> m = geo::ones;
    std::cout << m << std::endl;
    std::cout << geo::make_vector(2, 4) << std::endl;
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
