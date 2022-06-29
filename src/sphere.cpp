#include "sphere.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "aabb.hpp"

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    // TODO
    Vector3f R0 = r.getOrigin(); // 光线起始点
    Vector3f Rd = r.getDirection(); Rd.normalize(); // 光线单位方向向量
    Vector3f l = center - R0; // 由光源指向球心的向量
    if(l.squaredLength() == radius * radius){
        if(tmin <= 0 && 0 <= h.getT()) h.set(0, material, (R0 - center).normalized());
        return true; // 光源位于球面上，算作相交（光源即相交点）
    }
    
    float tp = l.dot(l, Rd); // 球心到光线所在直线的投影点 (垂足)
    if(l.squaredLength() > radius * radius && tp < 0) return false; // 如果光源在球体外部并且 tp < 0，那么光线与球面不相交

    float d = sqrt(l.squaredLength() - tp * tp); // 球心到光线所在直线的距离
    if(d > radius) return false; // 如果 d > r，那么光线与球面不相交
    
    float t_ = sqrt(radius * radius - d * d); // 投影点到光线与球面的交点的距离
    float t;
    Vector3f N; // 相交处法向（指向球外）
    if(l.squaredLength() > radius * radius){ // 光源在球体外部
        t = tp - t_;
        N = R0 + t * Rd - center;
    }
    else{ // 光源在球体内部
        t = tp + t_;
        N = R0 + t * Rd - center;
    }
    t = t / r.getDirection().length(); // Transform
    if(tmin < t && t < h.getT()){
        h.set(t, material, N);
        return true; // 不管光源在球体内部还是外部，都必定有交点
    }
    else return false;
}


bool Sphere::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    Vector3f oc = r.getOrigin() - center;
    auto a = r.getDirection().squaredLength();
    auto half_b = Vector3f::dot(oc, r.getDirection());
    auto c = oc.squaredLength() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.pointAtParameter(rec.t);
    Vector3f outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.material = material;

    return true;
}

bool Sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - Vector3f(radius, radius, radius),
        center + Vector3f(radius, radius, radius));
    return true;
}



Vector3f MovingSphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool MovingSphere::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    Vector3f oc = r.getOrigin() - center(r.time());
    auto a = r.getDirection().squaredLength();
    auto half_b = Vector3f::dot(oc, r.getDirection());
    auto c = oc.squaredLength() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.pointAtParameter(rec.t);
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.material = material;

    return true;
}

bool MovingSphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb box0(
        center(_time0) - Vector3f(radius, radius, radius),
        center(_time0) + Vector3f(radius, radius, radius));
    aabb box1(
        center(_time1) - Vector3f(radius, radius, radius),
        center(_time1) + Vector3f(radius, radius, radius));
    output_box = box0.surrounding_box(box0, box1);
    return true;
}
