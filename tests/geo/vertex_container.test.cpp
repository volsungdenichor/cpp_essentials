#include <catch.hpp>
#include <cpp_essentials/geo/vertex_container.hpp>

using namespace cpp_essentials;

TEST_CASE("triangle getters")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE(geo::vertex_count(t) == 3);
    REQUIRE(geo::segment_count(t) == 3);
    REQUIRE(geo::get_segment(t, 2) == geo::segment_2d<int>{ { 20, 0 }, { 0, 0 } });
    REQUIRE(geo::get_vertex(t, 2) == geo::vector_2d<int>{ 20, 0 });
}

TEST_CASE("segment getters")
{
    geo::segment_2d<int> t = { { 0, 0 }, { 10, 20 } };
    REQUIRE(geo::vertex_count(t) == 2);
    REQUIRE(geo::segment_count(t) == 1);
    REQUIRE(geo::get_segment(t, 0) == t);
    REQUIRE(geo::get_vertex(t, 1) == geo::vector_2d<int>{ 10, 20 });
}

TEST_CASE("rect getters")
{
    geo::rect_2d<int> t = { geo::vector_2d<int>{ 0, 0 }, geo::vector_2d<int>{ 10, 20 } };
    
    REQUIRE(geo::vertex_count(t) == 4);
    REQUIRE(geo::segment_count(t) == 4);
    
    REQUIRE(geo::get_vertex(t, 0) == geo::vector_2d<int>{ 0, 0 });
    REQUIRE(geo::get_vertex(t, 1) == geo::vector_2d<int>{ 9, 0 });
    REQUIRE(geo::get_vertex(t, 2) == geo::vector_2d<int>{ 9, 19 });
    REQUIRE(geo::get_vertex(t, 3) == geo::vector_2d<int>{ 0, 19 });

    REQUIRE(geo::get_segment(t, 0) == geo::segment_2d<int>{ geo::vector_2d<int>{ 0, 0 }, geo::vector_2d<int>{ 9, 0 } });
    REQUIRE(geo::get_segment(t, 1) == geo::segment_2d<int>{ geo::vector_2d<int>{ 9, 0 }, geo::vector_2d<int>{ 9, 19 } });
    REQUIRE(geo::get_segment(t, 2) == geo::segment_2d<int>{ geo::vector_2d<int>{ 9, 19 }, geo::vector_2d<int>{ 0, 19 } });
    REQUIRE(geo::get_segment(t, 3) == geo::segment_2d<int>{ geo::vector_2d<int>{ 0, 19 }, geo::vector_2d<int>{ 0, 0 } });
}