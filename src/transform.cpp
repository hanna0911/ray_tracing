#include "transform.hpp"
#include "aabb.hpp"
#include "ray.hpp"
#include "hit.hpp"


bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    Vector3f trSource = transformPoint(transform, r.getOrigin());
    Vector3f trDirection = transformDirection(transform, r.getDirection());
    Ray tr(trSource, trDirection);
    bool inter = o->intersect(tr, h, tmin);
    if (inter) {
        h.set(h.getT(), h.getMaterial(), transformDirection(transform.transposed(), h.getNormal()).normalized());
    }
    return inter;
}

// added functions

bool Transform::hit(const Ray& r, double tmin, double t_max, Hit& h) const {
	Vector3f trSource = transformPoint(transform, r.getOrigin());
    Vector3f trDirection = transformDirection(transform, r.getDirection());
    Ray tr(trSource, trDirection);
    bool inter = o->intersect(tr, h, tmin);
    if (inter) {
        h.set(h.getT(), h.getMaterial(), transformDirection(transform.transposed(), h.getNormal()).normalized());
    }
    return inter;
}

bool Transform::bounding_box(double time0, double time1, aabb& output_box) const { // 是否需要真正计算？（hit时候对光线变换了）
	aabb originalBox;
    o->bounding_box(time0, time1, originalBox);
    Vector3f minimum = transformPoint(transform, originalBox.minimum);
    Vector3f maximum = transformPoint(transform, originalBox.maximum);
	output_box = aabb(minimum, maximum);
	return true;
}
