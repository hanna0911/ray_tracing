#ifndef BOX_H
#define BOX_H

#include "utils.hpp"
#include "object3d.hpp"

class Group;
class aabb;
class xy_rect;
class xz_rect;
class yz_rect;

class box : public Object3D  {
    public:
        box() {}
        box(const Vector3f& p0, const Vector3f& p1, shared_ptr<Material> ptr);

        // Intersect Ray with this object. If hit, store information in hit structure.
        virtual bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

        virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        Vector3f box_min;
        Vector3f box_max;
        // Group *sides;
        shared_ptr<Group> sides;
};

#endif