#include "hit.hpp"
#include "ray.hpp"

void Hit::set_face_normal(const Ray& r, const Vector3f& outward_normal) {
    front_face = Vector3f::dot(r.getDirection(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}