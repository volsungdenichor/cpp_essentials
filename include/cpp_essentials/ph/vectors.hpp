#ifndef CPP_ESSENTIALS_PH_VECTORS_HPP_
#define CPP_ESSENTIALS_PH_VECTORS_HPP_

#include <cpp_essentials/ph/quantities.hpp>

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::ph
{

template <class Quant, class T, size_t R, size_t C>
using matrix = geo::matrix<quantity<Quant, T>, R, C>;

template <class Quant, class T, size_t D>
using vector = geo::vector<quantity<Quant, T>, D>;

template <class Quant, class T>
using vector_2d = vector<Quant, T, 2>;

template <class Quant, class T>
using vector_3d = vector<Quant, T, 3>;

template <class Quant, class T>
using interval = geo::interval<quantity<Quant, T>>;

template <class Quant, class T, size_t D>
using bounding_box = geo::bounding_box<quantity<Quant, T>, D>;

template <class Quant, class T>
using rect = bounding_box<Quant, T, 2>;

template <class Quant, class T>
using rect_2d = rect<Quant, T>;

template <class Quant, class T, size_t D>
using segment = geo::vector<quantity<Quant, T>, D>;

template <class Quant, class T>
using segment_2d = segment<Quant, T, 2>;

template <class Quant, class T>
using segment_3d = segment<Quant, T, 3>;

} /* namespace cpp_essentials::ph */

#endif /* CPP_ESSENTIALS_PH_VECTORS_HPP_ */

