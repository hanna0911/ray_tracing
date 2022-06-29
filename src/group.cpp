#include "group.hpp"
#include "aabb.hpp"
#include "hit.hpp"
#include "ray.hpp"

// added functions

bool Group::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    Hit temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool Group::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : output_box.surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

