#include "texture.hpp"
#include "perlin.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

Vector3f noise_texture::value(double u, double v, const Vector3f& p) const {
    return Vector3f(1,1,1) * 0.5 * (1 + sin(scale*p.z() + 10 * noise.turb(p)));
}

image_texture::image_texture(const char* filename) {
    auto components_per_pixel = bytes_per_pixel;

    data = stbi_load(
        filename, &width, &height, &components_per_pixel, components_per_pixel);

    if (!data) {
        std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}


checker_texture::checker_texture(Vector3f c1, Vector3f c2)
            : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)) {}

Vector3f checker_texture::value(double u, double v, const Vector3f& p) const {
    auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
    if (sines < 0)
        return odd->value(u, v, p);
    else
        return even->value(u, v, p);
}
