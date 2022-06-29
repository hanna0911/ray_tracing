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
    // std::cout << "hit transform object" << std::endl;
	Vector3f trSource = transformPoint(transform, r.getOrigin());
    Vector3f trDirection = transformDirection(transform, r.getDirection());
    Ray tr(trSource, trDirection);
    bool inter = o->hit(tr, tmin, t_max, h);
    if (inter) {
        // std::cout << "hit transform object" << std::endl;
        h.set(h.getT(), h.getMaterial(), transformDirection(transform.transposed(), h.getNormal()).normalized());
    }
    return inter;
}

bool Transform::bounding_box(double time0, double time1, aabb& output_box) const { // 是否需要真正计算？（hit时候对光线变换了）
	aabb originalBox;
    o->bounding_box(time0, time1, originalBox);
    Vector3f minimum = transformPoint(original, originalBox.minimum);
    Vector3f maximum = transformPoint(original, originalBox.maximum);
	output_box = aabb(minimum, maximum);
    std::cout << "min: " << minimum.x() << " " << minimum.y() << " " << minimum.z() << std::endl; 
    std::cout << "max: " << maximum.x() << " " << maximum.y() << " " << maximum.z() << std::endl; 
    return true;
}

