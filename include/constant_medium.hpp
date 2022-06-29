
#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "object3d.hpp"

class constant_medium : public Object3D {
    public:
        constant_medium(shared_ptr<Object3D> b, double d, shared_ptr<texture> a)
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(make_shared<isotropic>(a))
            {}

        constant_medium(shared_ptr<Object3D> b, double d, shared_ptr<Material> a) // added function
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(a)
            {}

        constant_medium(shared_ptr<Object3D> b, double d, Vector3f c)
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(make_shared<isotropic>(c))
            {}

        virtual bool intersect(const Ray &r, Hit &h, float tmin) override {return false;}

        virtual bool hit(
            const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            return boundary->bounding_box(time0, time1, output_box);
        }

    public:
        shared_ptr<Object3D> boundary;
        shared_ptr<Material> phase_function;
        double neg_inv_density;
};

#endif