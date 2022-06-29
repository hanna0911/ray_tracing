#ifndef BVH_H
#define BVH_H

#include "utils.hpp"
#include "object3d.hpp"
// #include "group.hpp"
#include <algorithm>
// #include "aabb.hpp"

class aabb;
class Group;

inline bool box_compare(const shared_ptr<Object3D> a, const shared_ptr<Object3D> b, int axis);

bool box_x_compare (const shared_ptr<Object3D> a, const shared_ptr<Object3D> b);

bool box_y_compare (const shared_ptr<Object3D> a, const shared_ptr<Object3D> b);

bool box_z_compare (const shared_ptr<Object3D> a, const shared_ptr<Object3D> b);

class bvh_node : public Object3D {
    public:
        bvh_node();

        /*
        bvh_node(const Group& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1)
        {}
        */

        bvh_node(
            const std::vector<shared_ptr<Object3D>>& src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(
            const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        virtual bool intersect(const Ray &r, Hit &h, float tmin) override;

    public:
        shared_ptr<Object3D> left;
        shared_ptr<Object3D> right;
        aabb *box;
};



#endif
