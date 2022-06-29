#ifndef AABB_H
#define AABB_H

#include "utils.hpp"
class Ray;

class aabb {
    public:
        aabb() {}
        aabb(const Vector3f& a, const Vector3f& b) { minimum = a; maximum = b;}

        Vector3f min() const {return minimum; }
        Vector3f max() const {return maximum; }

        bool hit(const Ray& r, double t_min, double t_max) const;

        aabb surrounding_box(aabb box0, aabb box1); // removed static

        Vector3f minimum;
        Vector3f maximum;
};

#endif