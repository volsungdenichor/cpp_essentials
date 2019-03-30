#ifndef CPP_ESSENTIALS_GX_DEFS_HPP_
#define CPP_ESSENTIALS_GX_DEFS_HPP_

#pragma once

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/geo/contains.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/circular_shape.hpp>

namespace cpp_essentials::gx
{

using point = geo::vector_2d<int>;
using vector = geo::vector_2d<int>;
using rect = geo::rect_2d<int>;
using triangle = geo::triangle_2d<int>;
using quad = geo::quad_2d<int>;
using segment = geo::segment_2d<int>;
using polygon = geo::polygon_2d<int>;
using polyline = geo::polyline_2d<int>;
using circle = geo::circle_2d<int>;

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_DEFS_HPP_ */
