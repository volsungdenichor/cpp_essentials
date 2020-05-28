#ifndef CPP_ESSENTIALS_PH_QUANTITIES_HPP_
#define CPP_ESSENTIALS_PH_QUANTITIES_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/tagged_value.hpp>

namespace cpp_essentials::ph
{

namespace detail
{

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct quantity_type
{
    static const int dimension0 = D0;
    static const int dimension1 = D1;
    static const int dimension2 = D2;
    static const int dimension3 = D3;
    static const int dimension4 = D4;
    static const int dimension5 = D5;
    static const int dimension6 = D6;
    static const int dimension7 = D7;
    static const int dimension8 = D8;
    static const int dimension9 = D9;
};

template <class T>
struct _is_quantity : std::false_type {};

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct _is_quantity<quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> : std::true_type {};

template <class T>
using is_quantity = std::enable_if_t<_is_quantity<T>::value>;


template <int D, class Quant>
struct get { };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<0, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D0; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<1, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D1; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<2, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D2; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<3, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D3; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<4, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D4; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<5, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D5; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<6, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D6; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<7, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D7; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<8, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D8; };

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
struct get<9, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>> { static const int value = D9; };

template
    < class Quant1
    , class Quant2
    , CONCEPT = is_quantity<Quant1>
    , CONCEPT = is_quantity<Quant2>>
struct mul_result
{
    using type = quantity_type
        < get<0, Quant1>::value + get<0, Quant2>::value
        , get<1, Quant1>::value + get<1, Quant2>::value
        , get<2, Quant1>::value + get<2, Quant2>::value
        , get<3, Quant1>::value + get<3, Quant2>::value
        , get<4, Quant1>::value + get<4, Quant2>::value
        , get<5, Quant1>::value + get<5, Quant2>::value
        , get<6, Quant1>::value + get<6, Quant2>::value
        , get<7, Quant1>::value + get<7, Quant2>::value
        , get<8, Quant1>::value + get<8, Quant2>::value
        , get<9, Quant1>::value + get<9, Quant2>::value>;
};

template
    < class Quant1
    , class Quant2
    , CONCEPT = is_quantity<Quant1>
    , CONCEPT = is_quantity<Quant2>>
struct div_result
{
    using type = quantity_type
        < get<0, Quant1>::value - get<0, Quant2>::value
        , get<1, Quant1>::value - get<1, Quant2>::value
        , get<2, Quant1>::value - get<2, Quant2>::value
        , get<3, Quant1>::value - get<3, Quant2>::value
        , get<4, Quant1>::value - get<4, Quant2>::value
        , get<5, Quant1>::value - get<5, Quant2>::value
        , get<6, Quant1>::value - get<6, Quant2>::value
        , get<7, Quant1>::value - get<7, Quant2>::value
        , get<8, Quant1>::value - get<8, Quant2>::value
        , get<9, Quant1>::value - get<9, Quant2>::value>;
};

template
    < class Quant
    , int N
    , int D
    , CONCEPT = is_quantity<Quant>>
struct pow_result
{
    using type = quantity_type
        < get<0, Quant>::value * N / D
        , get<1, Quant>::value * N / D
        , get<2, Quant>::value * N / D
        , get<3, Quant>::value * N / D
        , get<4, Quant>::value * N / D
        , get<5, Quant>::value * N / D
        , get<6, Quant>::value * N / D
        , get<7, Quant>::value * N / D
        , get<8, Quant>::value * N / D
        , get<9, Quant>::value * N / D>;
};

template <int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7, int D8, int D9>
std::ostream& operator <<(std::ostream& os, quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9> item)
{
    using type = quantity_type<D0, D1, D2, D3, D4, D5, D6, D7, D8, D9>;

    auto print = [&os](int dim, const char* name)
    {
        if (dim == 0)
        {
            return;
        }

        os << name;

        if (dim == 1)
        {
            return;
        }

        os << dim;
    };

    print(get<0, type>::value, "m");
    print(get<1, type>::value, "kg");
    print(get<2, type>::value, "s");
    print(get<3, type>::value, "px");
    print(get<4, type>::value, "K");
    print(get<5, type>::value, "rad");
    print(get<6, type>::value, "A");
    print(get<7, type>::value, "cd");
    print(get<8, type>::value, "sr");
    print(get<9, type>::value, "mol");

    return os;
}

template <class Quant1, class Quant2>
using mul = typename mul_result<Quant1, Quant2>::type;

template <class Quant1, class Quant2>
using div = typename div_result<Quant1, Quant2>::type;

template <class Quant, int Exp>
using pow = typename pow_result<Quant, Exp, 1>::type;

template <class Quant>
using inv = pow<Quant, -1>;

template <class Quant>
using sqr_root = typename pow_result<Quant, 1, 2>::type;


using scalar = quantity_type<0, 0, 0, 0, 0, 0, 0, 0, 0, 0>;

template <class Quant1, class Quant2, CONCEPT = is_quantity<Quant1>, CONCEPT = is_quantity<Quant2>>
auto operator *(Quant1, Quant2) -> mul<Quant1, Quant2>
{
    return {};
}

template <class Quant1, class Quant2, CONCEPT = is_quantity<Quant1>, CONCEPT = is_quantity<Quant2>>
auto operator /(Quant1, Quant2) -> div<Quant1, Quant2>
{
    return {};
}

template <class Quant, CONCEPT = is_quantity<Quant>>
auto operator /(int, Quant) -> inv<Quant>
{
    return {};
}

} /* namespace detail */

template
    < class Quant
    , class T
    , CONCEPT = detail::is_quantity<Quant>
    , CONCEPT = cc::Arithmetic<T>>
using quantity = core::tagged_value<T, Quant>;


template <class Quant, class T>
auto sqrt(quantity<Quant, T> item)
{
    return quantity<detail::sqr_root<Quant>, T>{ math::sqrt(item.get()) };
}

template <class Quant, class T>
auto sqr(quantity<Quant, T> item)
{
    return quantity<detail::pow<Quant, 2>, T>{ math::sqr(item.get()) };
}

template <class Quant, class T>
auto abs(quantity<Quant, T> item)
{
    return quantity<Quant, T>{ math::abs(item.get()) };
}

} /* namespace cpp_essentials::ph */

namespace std
{

using cpp_essentials::ph::sqrt;
using cpp_essentials::ph::abs;

} /* namespace std */

#endif /* CPP_ESSENTIALS_PH_QUANTITIES_HPP_ */
