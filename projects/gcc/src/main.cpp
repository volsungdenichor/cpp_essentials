#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <chrono>
#include <type_traits>
#include <random>

#include <json.hpp>

#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <cpp_essentials/gx/histogram_operations.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/math/polynomial.hpp>

#include <cpp_essentials/geo/bezier.hpp>
#include <cpp_essentials/geo/algorithm.hpp>

#include <cpp_essentials/proc/perlin.hpp>
#include <cpp_essentials/proc/distribution.hpp>

#include <cpp_essentials/geo/triangulation.hpp>
#include <cpp_essentials/geo/voronoi.hpp>

#include <cpp_essentials/gx/drawing_context.hpp>

#include <cpp_essentials/geo/coordinates_conversion.hpp>

#include <cpp_essentials/core/program_args.hpp>
#include <cpp_essentials/core/match.hpp>

#include <cpp_essentials/ph/ph.hpp>

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

    poisson_fn(geo::rect_2d<T> bounds, T r, int k, proc::seed_t seed = {})
        : poisson_fn{ bounds, (r / math::sqrt(T(2))) * geo::vector_2d<T>{ 1, 1 }, r, k, seed }
    {
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

private:
    void reset()
    {
        _points.clear();
        _active_samples.clear();
        _grid = not_set;
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

    static arrays::array_2d<sample_t>::size_type get_size(const geo::rect_2d<T>& bounds, const geo::vector_2d<T>& cell_size)
    {
        arrays::array_2d<sample_t>::size_type result;

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

template <class Func>
auto benchmark(std::string_view name, Func&& func)
{
    const auto start = std::chrono::high_resolution_clock::now();
    auto res = func();
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << name << ": " << elapsed << "ms" << std::endl;
    return res;
}

template <class T>
T from_file(const std::string& path)
{
    std::ifstream is{ path };
    T result;
    is >> result;
    return result;
}

nlohmann::json load_config(const std::string& path)
{
    std::cout << "loading config from " << path << std::endl;
    return from_file<nlohmann::json>(path);
}

nlohmann::json default_config()
{
    std::cout << "using default config" << std::endl;
    return {
        { "size", 400 },
        { "margin", 15 },
        {
            "perlin", {
                { "octaves", 10 },
                { "persistence", 0.2 },
                { "frequency", 0.2 },
                { "h_scale", 0.001 },
                { "v_scale", 0.001 },
                { "v_offset", 0.1 }
            }
        },
        {
            "poisson", {
                { "radius", 20.0 },
                { "k", 25 }
            }
        },
    };
}

void run(const nlohmann::json& config)
{
    std::cout << config.dump(2) << std::endl;

    const auto size = config["size"].get<int>();
    const auto margin = config["margin"].get<int>();

    auto poisson = poisson_fn<float>{
        { { -margin, -margin }, { size + margin, size + margin } },
        config["poisson"]["radius"],
        config["poisson"]["k"]
    };

    const auto perlin = proc::perlin_noise_ext<float>{
        proc::permutation{},
        config["perlin"]["octaves"],
        config["perlin"]["persistence"],
        config["perlin"]["frequency"]
    };

    const auto points = benchmark("points", [&] { return poisson(); });

    const auto delaunay = benchmark("delaunay", [&] { return geo::triangulate(points); });

    const auto voronoi = benchmark("voronoi", [&] { return geo::voronoi(delaunay); });

    const auto h_scale = config["perlin"]["h_scale"].get<float>();
    const auto v_scale = config["perlin"]["v_scale"].get<float>();
    const auto v_offset = config["perlin"]["v_offset"].get<float>();

    const auto float_image = benchmark("perlin", [&] { return generate_image<float>({ size, size }, [&](auto&& loc)
        {
            return perlin(loc * h_scale) * v_scale + v_offset;
        }); });

    auto image = std::invoke([&]() -> gx::rgb_image
        {
            const auto [min, max] = core::minmax_value(float_image);
            gx::rgb_image image{ { size, size } };
            core::transform(float_image, std::begin(image), [&](float v) { return gx::to_byte(256 * (v - min) / (max - min)); });
            return image;
        });

    auto context = gx::make_drawing_context(image.view());

    const auto p = proc::normal(192.F, 128.F);
    for (const auto& polygon : voronoi.faces() | sq::map([](const auto& face) { return face.as_polygon(); }))
    {
        const auto color = gx::rgb_color{ gx::to_byte(p()), gx::to_byte(p()), gx::to_byte(p()) };
        context.fill(polygon, color, 64);
        context.draw(polygon, gx::colors::dark_gray);
    };

    for (const auto p : points)
    {
        // context.draw_dot(p, gx::colors::red);
    }

    gx::save_bitmap(image, R"(out.bmp)");

}

int main(int argc, char** argv)
{
    const auto start = std::chrono::high_resolution_clock::now();
    try
    {
        const auto config = argc > 1
            ? load_config(argv[1])
            : default_config();

        run(config);
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
