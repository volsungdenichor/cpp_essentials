#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <chrono>
#include <type_traits>
#include <random>

#include <cpp_essentials/fmt/format.hpp>

#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <cpp_essentials/gx/histogram_operations.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/math/polynomial.hpp>

#include <cpp_essentials/geo/bezier.hpp>
#include <cpp_essentials/geo/algorithm.hpp>

#include <cpp_essentials/term/term.hpp>

#include <cpp_essentials/proc/perlin.hpp>
#include <cpp_essentials/proc/distribution.hpp>

#include <cpp_essentials/geo/triangulation.hpp>
#include <cpp_essentials/geo/voronoi.hpp>

#include <cpp_essentials/gx/drawing_context.hpp>

#include <cpp_essentials/geo/coordinates_conversion.hpp>

using namespace cpp_essentials;

template <class T, class Func>
gx::image<T> generate_image(const gx::image_size_t& size, Func func)
{
    gx::image<T> result{ size };
    for (auto it : core::views::iterate(result))
    {
        *it = func(it.location());
    }
    return result;
}

template <class T>
struct poisson_fn
{
    using sample_t = int;
    static constexpr sample_t not_set = -1;

    poisson_fn(geo::rect_2d<T> bounds, geo::vector_2d<T> cell_size, T r, int k, proc::seed_t seed = {})
        : _bounds{ std::move(bounds) }
        , _cell_size{ std::move(cell_size) }
        , _grid{ get_size(_bounds, _cell_size) }
        , _seed{ std::move(seed) }
        , _radius{ r }
        , _k{ k }
        , _radius_generator{ proc::uniform(_radius, 2 * _radius, _seed) }
        , _angle_generator{ proc::uniform(math::zero.as<T>(), math::two_pi.as<T>(), _seed) }
        , _x_generator{ proc::uniform(_bounds[0].lower(), _bounds[0].upper(), _seed) }
        , _y_generator{ proc::uniform(_bounds[1].lower(), _bounds[1].upper(), _seed) }
    {
        reset();
    }

    void reset()
    {
        _points.clear();
        _active_samples.clear();
        _grid = not_set;
    }

    std::vector<geo::vector_2d<T>> operator ()()
    {
        reset();

        add_point(random_point());

        while (!_active_samples.empty())
        {
            const auto sample_id = random_sample();
            const auto point = get_point(sample_id);

            if (const auto new_point = find_disc_point(point); new_point)
            {
                add_point(*new_point);
            }
            else
            {
                erase(sample_id);
            }
        }

        return _points;
    }

    void erase(sample_t sample_id)
    {
        core::erase_if(_active_samples, core::equal_to(sample_id));
    }

    sample_t random_sample()
    {
        const auto index = proc::uniform(0, (int)_active_samples.size() - 1, _seed);
        return _active_samples.at(index);
    }

    sample_t add_point(const geo::vector_2d<T>& point)
    {
        const auto sample_id = static_cast<sample_t>(_points.size());
        _active_samples.push_back(sample_id);
        _points.push_back(point);
        _grid.at(get_grid_location(point)) = sample_id;
        return sample_id;
    }

    static arrays::array_2d<int>::size_type get_size(const geo::rect_2d<T>& bounds, const geo::vector_2d<T>& cell_size)
    {
        arrays::array_2d<int>::size_type result;

        core::views::zip(bounds.size()._data, cell_size._data, core::divides)
            | sq::map(math::ceil)
            | sq::map(core::cast<int>)
            | sq::overwrite(result._data);

        return result;
    }

    arrays::array_2d<int>::location_type get_grid_location(geo::vector_2d<T> location) const
    {
        arrays::array_2d<int>::location_type result;

        location -= _bounds.lower();

        core::views::zip(location._data, _cell_size._data, core::divides)
            | sq::map(math::floor)
            | sq::map(core::cast<int>)
            | sq::overwrite(result._data);

        return result;
    }

    geo::vector_2d<T> random_point() const
    {
        return { _x_generator(), _y_generator() };
    }

    core::optional<geo::vector_2d<T>> find_disc_point(const geo::vector_2d<T>& center) const
    {
        for (auto&& _ : sq::range(_k))
        {
            const auto sample = random_point(center);
            if (is_valid(sample))
            {
                return sample;
            }
        }
        return core::none;
    }

    geo::vector_2d<T> random_point(const geo::vector_2d<T>& center) const
    {
        return center + geo::from_polar(_radius_generator(), _angle_generator());
    }

    const geo::vector_2d<T>& get_point(sample_t sample_id) const
    {
        return _points.at(sample_id);
    }

    bool is_valid(const geo::vector_2d<T>& sample) const
    {
        if (!geo::contains(_bounds, sample))
        {
            return false;
        }

        const auto grid_loc = get_grid_location(sample);

        if (const auto sample_id = _grid.at(grid_loc); sample_id != not_set)
        {
            return false;
        }

        static const auto margin = geo::vector_2d<int>{ 2, 2 };

        const auto rect = geo::make_intersection(
            geo::rect_2d<int>{ grid_loc - margin, grid_loc + margin },
            _grid.bounds());

        const auto r_sqr = math::sqr(_radius);

        return _grid.region(rect)
            | sq::drop_if(core::equal_to(not_set))
            | sq::all_of([&](sample_t sample_id) { return geo::norm(get_point(sample_id) - sample) > r_sqr; });
    }

    geo::rect_2d<T> _bounds;
    geo::vector_2d<T> _cell_size;
    arrays::array_2d<sample_t> _grid;
    proc::seed_t _seed;
    T _radius;
    int _k;

    std::vector<geo::vector_2d<T>> _points;
    std::vector<sample_t> _active_samples;

    core::generator<T> _radius_generator;
    core::generator<T> _angle_generator;
    core::generator<T> _x_generator;
    core::generator<T> _y_generator;
};

void run()
{
    static const auto rnd = proc::uniform(-50.F, 450.F);

    static const auto create_vertex = [&]() -> geo::vector_2d<float>
    {
        return { rnd(), rnd() };
    };

    float r = 20.F;
    float s = r / math::sqrt(2.F);

    poisson_fn<float> poisson{ { { -5, -5 }, { 405, 405 } }, { s, s }, r, 25 };

    //const auto points = core::make_infinite_generator(create_vertex) | sq::take(50) | sq::to_vector();
    const auto points = poisson();

    gx::rgb_image image{ { 400, 400 } };

    auto ctx = gx::make_drawing_context(image.view());


#if 1
    const auto voronoi = geo::voronoi(points);

    for (const auto f : voronoi.faces())
    {
        ctx.draw(f.as_polygon(), gx::colors::white);
    }
#endif

    for (const auto p : points)
    {
        ctx.draw_dot(p, gx::colors::red);
    }

    char ch;
    (void)ch;

}

int main(int argc, char** argv)
{
    const auto start = std::chrono::high_resolution_clock::now();
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Elapsed " << elapsed << "ms" << std::endl;
    return 0;
}
