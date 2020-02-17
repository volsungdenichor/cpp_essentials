#ifndef CPP_ESSENTIALS_PROC_PERLIN_HPP_
#define CPP_ESSENTIALS_PROC_PERLIN_HPP_

#pragma once

#include <vector>

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/functors.hpp>

#include <cpp_essentials/math/polynomial.hpp>

#include <cpp_essentials/geo/matrix.hpp>

#include <cpp_essentials/sq/sq.hpp>

#include <cpp_essentials/proc/seed.hpp>

namespace cpp_essentials::proc
{

class permutation
{
public:
    permutation(std::vector<int> values)
        : _values{ std::move(values) }
    {
        EXPECTS(_values.size() == 256);
    }

    permutation(seed_t seed = {})
        : permutation{ generate(seed) }
    {
    }

    permutation(const permutation&) = default;
    permutation(permutation&&) = default;

    int operator [](size_t index) const
    {
        return _values[index % 256];
    }

private:
    static std::vector<int> generate(seed_t seed)
    {
        std::vector<int> result;
        result.resize(256);
        core::iota(result, 0);
        core::shuffle(result, seed.template to_generator<>());
        return result;
    }

    std::vector<int> _values;
};

class perlin_noise
{
public:
    perlin_noise(permutation permutation)
        : _permutation{ std::move(permutation) }
    {
    }

    template <class T>
    T operator ()(const geo::vector_3d<T>& location) const
    {
        using core::lerp;

        static const std::array<geo::vector<int, 3>, 8> translations =
        {
            geo::vector_3d<int>{ 0, 0, 0 },
            geo::vector_3d<int>{ 1, 0, 0 },
            geo::vector_3d<int>{ 0, 1, 0 },
            geo::vector_3d<int>{ 1, 1, 0 },
            geo::vector_3d<int>{ 0, 0, 1 },
            geo::vector_3d<int>{ 1, 0, 1 },
            geo::vector_3d<int>{ 0, 1, 1 },
            geo::vector_3d<int>{ 1, 1, 1 },
        };

        static const auto fade = [](T v)
        {
            static const math::polynomial<T, 5> poly{ 0, 0, 0, +10, -15, +6 };
            return poly(v);
        };

        const auto pos = geo::vector_3d<int>{ location._data | sq::map(math::floor) };
        const auto rel_pos = geo::vector_3d<T>{ location._data | sq::map(math::fractional_part) };
        const auto p = geo::vector_3d<T>{ rel_pos._data | sq::map(fade) };

        const auto vert = std::invoke([&]()
        {
            std::array<int, 2> v;
            sq::range(2) | sq::map([&](int i) { return _permutation[pos[0] + i] + pos[1]; }) | sq::overwrite(v);

            std::array<int, 4> vert;
            sq::range(4) | sq::map([&](int i) { return _permutation[v[i % 2] + (i / 2)] + pos[2]; }) | sq::overwrite(vert);
            return vert;
        });

        const auto level_0 = [&](int a, int b)
        {
            a *= 2;
            int c = 4 * b + a;
            return lerp(p[0],
                grad(_permutation[vert[a + 0] + b], rel_pos - translations[c + 0]),
                grad(_permutation[vert[a + 1] + b], rel_pos - translations[c + 1]));
        };

        const auto level_1 = [&](int a)
        {
            return lerp(p[1], level_0(0, a), level_0(1, a));
        };

        return lerp(p[2], level_1(0), level_1(1));
    }

    template <class T>
    T operator  ()(const geo::vector_2d<T>& location) const
    {
        return (*this)(convert(location));
    }

    template <class T>
    static geo::vector_3d<T> convert(const geo::vector_2d<T>& v)
    {
        return { v.x(), v.y(), T{} };
    }

private:
    template <class T>
    static T grad(int hash, T x, T y, T z)
    {
        int h = hash & 0xF;

        T u = h < 0x08 ? x : y;
        T v = h < 4 ? y : h == 0x0c || h == 0x0e ? x : z;

        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    template <class T>
    static T grad(int hash, const geo::vector<T, 3>& vect)
    {
        return grad(hash, vect[0], vect[1], vect[2]);
    }

    permutation _permutation;
};

template <class T>
class perlin_noise_ext
{
public:
    perlin_noise_ext(perlin_noise inner, int octaves, T persistence, T frequency)
        : _inner{ std::move(inner) }
        , _octaves{ octaves }
        , _persistence{ persistence }
        , _frequency{ frequency }
    {
    }

    perlin_noise_ext(permutation permutation, int octaves, T persistence, T frequency)
        : perlin_noise_ext{ perlin_noise{ std::move(permutation) }, octaves, persistence, frequency }
    {
    }

    template <class T>
    T operator()(const geo::vector_3d<T>& location) const
    {
        T sum = math::zero;
        T amplitude = math::one;
        T f = _frequency;

        for (auto _ : sq::range(_octaves))
        {
            sum += _inner(location * f) * amplitude;
            amplitude *= _persistence;
            f *= 2;
        }

        T max_value = amplitude * _octaves;

        return sum / max_value;
    }


    template <class T>
    T operator()(const geo::vector_2d<T>& location) const
    {
        return (*this)(perlin_noise::convert(location));
    }

private:
    perlin_noise _inner;
    int _octaves;
    T _persistence;
    T _frequency;
};

} /* namespace cpp_essentials::proc */

#endif /* CPP_ESSENTIALS_PROC_PERLIN_HPP_ */
