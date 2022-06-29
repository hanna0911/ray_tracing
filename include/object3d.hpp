#ifndef OBJECT3D_H
#define OBJECT3D_H

// #include "ray.hpp"
// #include "hit.hpp"
#include "material.hpp"
// #include "aabb.hpp"

class Ray;
class Hit;
class aabb;

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    virtual ~Object3D() = default;

    explicit Object3D(shared_ptr<Material> material) {
    // explicit Object3D(Material *material) {
        this->material = material;
    }

    // Intersect Ray with this object. If hit, store information in hit structure.
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    // added functions
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;

protected:

    shared_ptr<Material> material; // Material *material;
};

#endif

