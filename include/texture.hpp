#ifndef TEXTURE_H
#define TEXTURE_H

// #include "stb_image.hpp"
#include <iostream>
#include "utils.hpp"

class perlin; 

class texture {
    public:
        virtual Vector3f value(double u, double v, const Vector3f& p) const = 0;
};


class checker_texture : public texture {
    public:
        checker_texture() {}

        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(_even), odd(_odd) {}

        checker_texture(Vector3f c1, Vector3f c2);

        virtual Vector3f value(double u, double v, const Vector3f& p) const override;

    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;
};


class solid_color : public texture {
    public:
        solid_color() {}
        solid_color(Vector3f c) : color_value(c) {}

        solid_color(double red, double green, double blue)
          : solid_color(Vector3f(red,green,blue)) {}

        virtual Vector3f value(double u, double v, const Vector3f& p) const override {
            return color_value;
        }

    private:
        Vector3f color_value;
};


class noise_texture : public texture {
    public:
        noise_texture() {}
        noise_texture(double sc) : scale(sc) {}

        virtual Vector3f value(double u, double v, const Vector3f& p) const override;

    public:
        perlin *noise;
        double scale;
};


class image_texture : public texture {
    public:
        const static int bytes_per_pixel = 3;

        image_texture()
          : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        image_texture(const char* filename);
        
        ~image_texture() {
            delete data;
        }

        virtual Vector3f value(double u, double v, const Vector3f& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return Vector3f(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width-1;
            if (j >= height) j = height-1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

            return Vector3f(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }

    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;
};

#endif