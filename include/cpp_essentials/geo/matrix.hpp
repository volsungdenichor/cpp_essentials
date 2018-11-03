#ifndef CPP_ESSENTIALS_GEO_MATRIX_HPP_
#define CPP_ESSENTIALS_GEO_MATRIX_HPP_

#pragma once

#include <cpp_essentials/math/matrix.hpp>

namespace cpp_essentials::geo
{

using math::matrix;
using math::square_matrix;
using math::square_matrix_2d;
using math::square_matrix_3d;
using math::vector;
using math::vector_2d;
using math::vector_3d;

using math::elementwise_divide;
using math::elementwise_multiply;

using math::make_vector;
using math::as_tuple;

using math::identity;
using math::ones;
using math::zeros;

using math::minor;
using math::determinant;
using math::is_invertible;
using math::invert;
using math::transpose;

using math::dot;
using math::norm;
using math::length;
using math::normalize;
using math::unit;
using math::distance;
using math::cross;
using math::projection;
using math::rejection;
using math::perpendicular;
using math::angle;
using math::bisector;

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_MATRIX_HPP_ */
