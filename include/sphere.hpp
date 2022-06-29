#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        center = Vector3f(0, 0, 0);
    }

    Sphere(const Vector3f &center, float radius, shared_ptr<Material> material) : Object3D(material) {
    // Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        // TODO
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    
    // added functions
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    
protected:
    Vector3f center; // 球心
    float radius; // 半径

private:
    // 纹理UV映射
    static void get_sphere_uv(const Vector3f& p, float& u, float& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};


#endif
