#ifndef CPP_ESSENTIALS_PROC_DISTIBUTION_HPP_
#define CPP_ESSENTIALS_PROC_DISTIBUTION_HPP_

#pragma once

#include <cpp_essentials/proc/seed.hpp>

namespace cpp_essentials::proc
{

namespace detail
{

template <class Distr, class Generator = std::default_random_engine>
class distr_wrapper
{
public:
    using result_type = typename Distr::result_type;

    distr_wrapper(Distr distr, Generator gen)
        : _distr{ std::move(distr) }
        , _gen{ std::move(gen) }
    {
    }

    result_type operator ()() const
    {
        return _distr(_gen);
    }

    operator result_type() const
    {
        return _distr(_gen);
    }

private:
    mutable Distr _distr;
    mutable Generator _gen;
};

template <class Distr, class Generator = std::default_random_engine, class... Args>
auto make_distr_wrapper(seed_t seed, Args&&... args) -> distr_wrapper<Distr>
{
    return { Distr { FORWARD(args)... }, seed.template to_generator<Generator>() };
}

template <class T>
using ensure_floating_point = std::conditional_t<std::is_floating_point<T>::value, T, double>;

template <class T>
using ensure_integral = std::conditional_t<std::is_integral<T>::value, T, int>;

struct normal_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T mean, T std_dev, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::normal_distribution<U>>(seed, static_cast<U>(mean), static_cast<U>(std_dev));
    }
};

struct uniform_fn
{
    template <class T>
    auto operator ()(T min, T max, seed_t seed = {}) const
    {
        using distr_type = std::conditional_t
            < std::is_floating_point<T>::value
            , std::uniform_real_distribution<T>
            , std::uniform_int_distribution<T>>;

        return make_distr_wrapper<distr_type>(seed, min, max);
    }
};

struct chi_squared_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T n, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::chi_squared_distribution<U>>(seed, static_cast<U>(n));
    }
};

struct bernoulli_fn
{
    auto operator ()(double p, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::bernoulli_distribution>(seed, p);
    }
};

struct binomial_fn
{
    template <class T, class U = ensure_integral<T>>
    auto operator ()(T t, double p, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::binomial_distribution<U>>(seed, static_cast<U>(t), p);
    }
};

struct negative_binomial_fn
{
    template <class T, class U = ensure_integral<T>>
    auto operator ()(T t, double p, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::negative_binomial_distribution<U>>(seed, static_cast<U>(t), p);
    }
};

struct geometric_fn
{
    auto operator ()(double p, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::geometric_distribution<>>(seed, p);
    }
};

struct poisson_fn
{
    auto operator ()(double p, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::poisson_distribution<>>(seed, p);
    }
};

struct exponential_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T lambda, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::exponential_distribution<U>>(seed, static_cast<U>(lambda));
    }
};

struct gamma_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T alpha, T beta, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::gamma_distribution<U>>(seed, static_cast<U>(alpha), static_cast<U>(beta));
    }
};

struct weibull_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T a, T b, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::weibull_distribution<U>>(seed, static_cast<U>(a), static_cast<U>(b));
    }
};

struct extreme_value_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T a, T b, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::extreme_value_distribution<U>>(seed, static_cast<U>(a), static_cast<U>(b));
    }
};

struct lognormal_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T m, T s, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::lognormal_distribution<U>>(seed, static_cast<U>(m), static_cast<U>(s));
    }
};

struct cauchy_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T a, T b, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::cauchy_distribution<U>>(seed, static_cast<U>(a), static_cast<U>(b));
    }
};

struct fisher_f_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T m, T n, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::fisher_f_distribution<U>>(seed, static_cast<U>(m), static_cast<U>(n));
    }
};

struct student_t_fn
{
    template <class T, class U = ensure_floating_point<T>>
    auto operator ()(T n, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::student_t_distribution<U>>(seed, static_cast<U>(n));
    }
};

struct discrete_fn
{
    auto operator ()(std::initializer_list<double> weights, seed_t seed = {}) const
    {
        return make_distr_wrapper<std::discrete_distribution<>>(seed, weights);
    }
};

} /* namespace detail */

static constexpr auto normal = detail::normal_fn{};
static constexpr auto uniform = detail::uniform_fn{};
static constexpr auto chi_squared = detail::chi_squared_fn{};
static constexpr auto bernoulli = detail::bernoulli_fn{};
static constexpr auto binomial = detail::binomial_fn{};
static constexpr auto negative_binomial = detail::negative_binomial_fn{};
static constexpr auto geometric = detail::geometric_fn{};
static constexpr auto poisson = detail::poisson_fn{};
static constexpr auto exponential = detail::exponential_fn{};
static constexpr auto gamma = detail::gamma_fn{};
static constexpr auto weibull = detail::weibull_fn{};
static constexpr auto extreme_value = detail::extreme_value_fn{};
static constexpr auto lognormal = detail::lognormal_fn{};
static constexpr auto cauchy = detail::cauchy_fn{};
static constexpr auto fisher_f = detail::fisher_f_fn{};
static constexpr auto student_t = detail::student_t_fn{};
static constexpr auto discrete = detail::discrete_fn{};

} /* namespace cpp_essentials::proc */

#endif /* CPP_ESSENTIALS_PROC_DISTIBUTION_HPP_ */
