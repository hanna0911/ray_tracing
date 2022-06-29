#ifndef BOX_H
#define BOX_H

#include "utils.hpp"

#include "plane.hpp"
#include "group.hpp"
#include "aabb.hpp"

class box : public Object3D  {
    public:
        box() {}
        box(const Vector3f& p0, const Vector3f& p1, shared_ptr<Material> ptr);

        virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = aabb(box_min, box_max);
            return true;
        }

    public:
        Vector3f box_min;
        Vector3f box_max;
        Group sides;
};

box::box(const Vector3f& p0, const Vector3f& p1, shared_ptr<Material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}

#endif