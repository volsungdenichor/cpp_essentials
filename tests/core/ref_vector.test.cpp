#include <catch.hpp>
#include <cpp_essentials/core/ref_vector.hpp>

using namespace cpp_essentials;

TEST_CASE("ref_vector")
{
    int x = 10;
    int y = 42;

    core::ref_vector<int> vect;
    REQUIRE(vect.empty() == true);

    vect.push_back(x);
    vect.push_back(y);
    vect.push_back(x);

    REQUIRE(vect.empty() == false);
    REQUIRE(vect.size() == 3);

    REQUIRE(&vect[0]== &x);
    REQUIRE(&vect[1] == &y);
    REQUIRE(&vect[2] == &x);   
    REQUIRE(&*vect.begin() == &x);
}


TEST_CASE("ref_vector::ctor")
{
    std::vector<int> original = { 2, 4, 8 };

    core::ref_vector<int> vect{ original };
    REQUIRE(vect.empty() == false);
    REQUIRE(vect.size() == original.size());

    for (size_t i = 0; i < vect.size(); ++i)
    {
        REQUIRE(&vect[i] == &original[i]);
    }
}
