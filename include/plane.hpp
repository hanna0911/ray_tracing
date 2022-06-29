#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include "utils.hpp"
// #include "aabb.hpp"
#include <vecmath.h>
#include <cmath>

class Hit;
class aabb;
class Ray;

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

/*
class Plane : public Object3D {
public:
    Plane() {
        this->normal = Vector3f(0, 0, 0);
        this->D = 0;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal; 
        this->D = -d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f R0 = r.getOrigin(); // 光线起始点
        Vector3f Rd = r.getDirection(); Rd.normalize(); // 光线单位方向向量
        float t = - ((D + normal.dot(normal, R0)) / normal.dot(normal, Rd));
        t = t / r.getDirection().length(); // Transform
        if(tmin < t && t < h.getT()){
            h.set(t, material, normal);
            return true; // 不管光源在球体内部还是外部，都必定有交点
        }
        else return false;
    }

protected:
    float D; // n·P + D = 0 
    Vector3f normal; // 法向量
};
*/

// added planes (finite)

class xy_rect : public Object3D {
    public:
        xy_rect() {}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, 
            shared_ptr<Material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};


        virtual bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

        virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<Material> mp;
        double x0, x1, y0, y1, k;
};

class xz_rect : public Object3D {
    public:
        xz_rect() {}

        xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
            shared_ptr<Material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }
        
        virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<Material> mp;
        double x0, x1, z0, z1, k;
};

class yz_rect : public Object3D {
    public:
        yz_rect() {}

        yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
            shared_ptr<Material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }
        
        virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<Material> mp;
        double y0, y1, z0, z1, k;
};


#endif //PLANE_H
		

