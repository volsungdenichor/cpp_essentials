#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/chunk.hpp>

#include <cpp_essentials/sq/sq.hpp>

using namespace cpp_essentials;

static constexpr core::adaptable<core::detail::pairwise_t> pairwise = {};

void run()
{
    std::vector<int> vect = { 1, 0, 5, 4, 3, 7 };
    sq::range(10)
        | sq::backwards()
        | sq::drop(2)
        | sq::stride(2)
        | sq::map(core::to_string)

        | sq::copy(core::output(std::cout, " "))
        ;
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
