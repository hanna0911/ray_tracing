#include "camera.hpp"
#include "ray.hpp"

Ray Camera::generateRay(const Vector2f &point) const {
    float s = point.x(), t = point.y();
    return Ray(
        origin,
        lower_left_corner + s * horizontal + t * vertical - origin,
        random_double(time0, time1)
    );
}

Ray Camera::generateRandomRay(const Vector2f &point) const {
    float s = point.x(), t = point.y();
    Vector3f rd = lens_radius * random_in_unit_disk();
    Vector3f offset = u * rd.x() + v * rd.y();

    return Ray(
        origin + offset,
        lower_left_corner + s * horizontal + t * vertical - origin - offset,
        random_double(time0, time1)
    );
}