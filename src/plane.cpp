#include "plane.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "aabb.hpp"


bool xz_rect::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    auto t = (k-r.getOrigin().y()) / r.getDirection().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.getOrigin().x() + t*r.getDirection().x();
    auto z = r.getOrigin().z() + t*r.getDirection().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = Vector3f(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.material = mp;
    rec.p = r.pointAtParameter(t);
    return true;
}

bool yz_rect::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    auto t = (k-r.getOrigin().x()) / r.getDirection().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.getOrigin().y() + t*r.getDirection().y();
    auto z = r.getOrigin().z() + t*r.getDirection().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = Vector3f(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.material = mp;
    rec.p = r.pointAtParameter(t);
    return true;
}


bool xy_rect::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    auto t = (k-r.getOrigin().z()) / r.getDirection().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.getOrigin().x() + t*r.getDirection().x();
    auto y = r.getOrigin().y() + t*r.getDirection().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = Vector3f(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.material = mp;
    rec.p = r.pointAtParameter(t);
    return true;
}

bool yz_rect::bounding_box(double time0, double time1, aabb& output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    output_box = aabb(Vector3f(k-0.0001, y0, z0), Vector3f(k+0.0001, y1, z1));
    return true;
}

bool xz_rect::bounding_box(double time0, double time1, aabb& output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    output_box = aabb(Vector3f(x0,k-0.0001,z0), Vector3f(x1, k+0.0001, z1));
    return true;
}

bool xy_rect::bounding_box(double time0, double time1, aabb& output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    output_box = aabb(Vector3f(x0,y0, k-0.0001), Vector3f(x1, y1, k+0.0001));
    return true;
}