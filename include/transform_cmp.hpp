
#include "utils.hpp"
#include "aabb.hpp"
#include "object3d.hpp"
// #include "hit.hpp"
// #include "ray.hpp"

class Ray;
class Hit;

class translate : public Object3D {
    public:
        translate(shared_ptr<Object3D> p, const Vector3f& displacement)
            : ptr(p), offset(displacement) {}


        virtual bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

        virtual bool hit(
            const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<Object3D> ptr;
        Vector3f offset;
};

class rotate_y : public Object3D {
    public:
        rotate_y(shared_ptr<Object3D> p, double angle);

        virtual bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

        virtual bool hit(
            const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        shared_ptr<Object3D> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;
};
