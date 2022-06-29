#ifndef CAMERA_H
#define CAMERA_H

// #include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>
#include "utils.hpp"

class Ray;

class Camera {
public:
    Camera(
        int _width,
        int _height,
        Vector3f lookfrom,
        Vector3f lookat,
        Vector3f vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist, 
        double _time0 = 0,
        double _time1 = 0
    ) {
        image_width = _width;
        image_height = _height;

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = (lookfrom - lookat).normalized();
        u = (Vector3f::cross(vup, w)).normalized();
        v = Vector3f::cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
        
        // Motion Blur
        time0 = _time0;
        time1 = _time1;
    }

    Ray generateRay(const Vector2f &point) const;

    Ray generateRandomRay(const Vector2f &point) const;

    int getWidth() const { return image_width; }
    int getHeight() const { return image_height; }

private:
    int image_width, image_height;
    Vector3f origin;
    Vector3f lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f u, v, w;
    double lens_radius;
    double time0, time1;  // shutter open/close times
};


#endif //CAMERA_H
