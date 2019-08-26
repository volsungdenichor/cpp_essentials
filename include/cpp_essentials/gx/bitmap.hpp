#ifndef CPP_ESSENTIALS_GX_BITMAP_HPP_
#define CPP_ESSENTIALS_GX_BITMAP_HPP_

#include <fstream>

#include <cpp_essentials/core/algorithm.hpp>

#include <cpp_essentials/gx/image.hpp>

namespace cpp_essentials::gx
{

namespace detail
{

template <class T, class U = T>
void write(std::ostream& os, const U& value)
{
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <class T>
T read(std::istream& is)
{
    T value;
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
    return value;
}

inline void write_n(std::ostream& os, size_t count, char value = 0)
{
    for (size_t i = 0; i < count; ++i)
    {
        write<char>(os, value);
    }
}

inline size_t get_padding(size_t width, size_t bits_per_pixel)
{
    return ((bits_per_pixel * width + 31) / 32) * 4 - (width * bits_per_pixel / 8);
}

struct bmp_header
{
    static const size_t size = 14;

    bmp_header()
        : file_size(0)
        , data_offset(0)
    {
    }

    void save(std::ostream& os) const
    {
        write<char>(os, 'B');
        write<char>(os, 'M');
        write<int>(os, file_size);
        write<short>(os, 0);                /* reserved1 */
        write<short>(os, 0);                /* reserved2 */
        write<int>(os, data_offset);
    }

    void load(std::istream& is)
    {
        read<char>(is);                     /* B */
        read<char>(is);                     /* M */;
        file_size = read<int>(is);
        read<short>(is);                    /* reserved1 */
        read<short>(is);                    /* reserved2 */
        read<int>(is);                      /* data_offset */
    }

    size_t file_size;
    size_t data_offset;
};

struct dib_header
{
    static const size_t size;

    dib_header()
        : width(0)
        , height(0)
        , color_plane_count(0)
        , bits_per_pixel(0)
        , compression(0)
        , data_size(0)
        , horizontal_pixel_per_meter(0)
        , vertical_pixel_per_meter(0)
        , color_count(0)
        , important_color_count(0)
    {
    }

    size_t width;
    size_t height;
    size_t color_plane_count;
    size_t bits_per_pixel;
    size_t compression;
    size_t data_size;
    size_t horizontal_pixel_per_meter;
    size_t vertical_pixel_per_meter;
    size_t color_count;
    size_t important_color_count;

    void save(std::ostream& os) const
    {
        write<int>(os, size);
        write<int>(os, width);
        write<int>(os, height);
        write<short>(os, color_plane_count);
        write<short>(os, bits_per_pixel);
        write<int>(os, compression);
        write<int>(os, data_size);
        write<int>(os, horizontal_pixel_per_meter);
        write<int>(os, vertical_pixel_per_meter);
        write<int>(os, color_count);
        write<int>(os, important_color_count);
    }

    void load(std::istream& is)
    {
        read<int>(is);                      /* size */
        width = read<int>(is);
        height = read<int>(is);
        color_plane_count = read<short>(is);
        bits_per_pixel = read<short>(is);
        compression = read<int>(is);
        data_size = read<int>(is);
        horizontal_pixel_per_meter = read<int>(is);
        vertical_pixel_per_meter = read<int>(is);
        color_count = read<int>(is);
        important_color_count = read<int>(is);
    }
};

const size_t dib_header::size = 40;

inline void save_header(
    std::ostream& os,
    size_t width,
    size_t height,
    size_t padding,
    size_t bits_per_pixel,
    size_t palette_size)
{
    size_t data_size = (width + padding) * height * (bits_per_pixel / 8);
    size_t data_offset = bmp_header::size + dib_header::size + palette_size;
    size_t file_size = data_offset + data_size;

    bmp_header bmp_hdr;
    bmp_hdr.file_size = file_size;
    bmp_hdr.data_offset = data_offset;

    dib_header dib_hdr;
    dib_hdr.width = width;
    dib_hdr.height = height;
    dib_hdr.color_plane_count = 1;
    dib_hdr.bits_per_pixel = bits_per_pixel;
    dib_hdr.compression = 0;
    dib_hdr.data_size = data_size;

    bmp_hdr.save(os);
    dib_hdr.save(os);
}

inline rgb_image load_bitmap_8(const detail::dib_header& header, std::istream& is)
{
    rgb_image result({ header.width, header.height });

    auto padding = detail::get_padding(result.width(), header.bits_per_pixel);

    std::array<rgb_color, 256> palette;

    for (size_t i = 0; i < 256; ++i)
    {
        auto b = detail::read<byte>(is);
        auto g = detail::read<byte>(is);
        auto r = detail::read<byte>(is);

        is.ignore(1);

        palette[i] = { r, g, b };
    }

    for (int y = int(result.height()) - 1; y >= 0; --y)
    {
        auto row = result[y];

        for (int x = 0; x < result.width(); ++x)
        {
            auto color = detail::read<byte>(is);

            row[x] = palette[color];
        }

        is.ignore(padding);
    }

    return result;
}

inline rgb_image load_bitmap_24(const detail::dib_header& header, std::istream& is)
{
    rgb_image result({ header.width, header.height });

    auto padding = detail::get_padding(result.width(), header.bits_per_pixel);

    for (int y = int(result.height()) - 1; y >= 0; --y)
    {
        auto row = result[y];

        for (int x = 0; x < (int)result.width(); ++x)
        {
            auto b = detail::read<byte>(is);
            auto g = detail::read<byte>(is);
            auto r = detail::read<byte>(is);

            row[x] = rgb_color(r, g, b);
        }

        is.ignore(padding);
    }

    return result;
}

struct save_bitmap_fn
{
    void operator ()(byte_image::const_view_type view, std::ostream& os) const
    {
        size_t bits_per_pixel = 8;
        auto padding = detail::get_padding(view.width(), bits_per_pixel);

        detail::save_header(os, view.width(), view.height(), padding, bits_per_pixel, 256 * 4);

        for (size_t i = 0; i < 256; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                detail::write<char>(os, i);
            }

            detail::write<char>(os, 0);
        }

        for (int y = (int)view.height() - 1; y >= 0; --y)
        {
            auto row = view[y];

            for (int x = 0; x < view.width(); ++x)
            {
                auto value = row[x];

                detail::write<byte>(os, value);
            }

            detail::write_n(os, padding);
        }
    }

    void operator ()(byte_image::const_view_type view, const std::string& file) const
    {
        std::ofstream fs(file.c_str(), std::ofstream::binary);

        (*this)(view, fs);
    }

    void operator ()(rgb_image::const_view_type view, std::ostream& os) const
    {
        size_t bits_per_pixel = 24;
        size_t padding = detail::get_padding(view.width(), bits_per_pixel);

        detail::save_header(os, view.width(), view.height(), padding, bits_per_pixel, 0);

        for (int y = int(view.height()) - 1; y >= 0; --y)
        {
            auto row = view[y];

            for (int x = 0; x < view.width(); ++x)
            {
                auto value = row[x];

                detail::write<byte>(os, value.blue());
                detail::write<byte>(os, value.green());
                detail::write<byte>(os, value.red());
            }

            detail::write_n(os, padding);
        }
    }

    void operator ()(rgb_image::const_view_type view, const std::string& file) const
    {
        std::ofstream fs(file.c_str(), std::ofstream::binary);

        (*this)(view, fs);
    }
};

struct load_bitmap_fn
{
    rgb_image operator ()(std::istream& is) const
    {
        if (!is)
        {
            throw std::runtime_error{ "load_bitmap: invalid stream" };
        }

        detail::bmp_header bmp_hdr;
        detail::dib_header dib_hdr;

        bmp_hdr.load(is);
        dib_hdr.load(is);

        switch (dib_hdr.bits_per_pixel)
        {
            case 8:
                return detail::load_bitmap_8(dib_hdr, is);

            case 24:
                return detail::load_bitmap_24(dib_hdr, is);

            default:
                throw std::runtime_error{ "load_bitmap: format not supported" };

        }
    }

    rgb_image operator ()(const std::string& file) const
    {
        std::ifstream fs(file.c_str(), std::ifstream::binary);

        return (*this)(fs);
    }
};

} /* namespace detail */

static constexpr detail::save_bitmap_fn save_bitmap = {};
static constexpr detail::load_bitmap_fn load_bitmap = {};

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_BITMAP_HPP_ */
