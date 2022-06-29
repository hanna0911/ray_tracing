#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
// #include "ray.hpp"
#include "utils.hpp"

class Ray;
class Material;

class Hit {
public:

    // constructors
    Hit() {
        material = nullptr;
        t = 1e38;
    }

    Hit(float _t, shared_ptr<Material> m, const Vector3f &n) { // Hit(float _t, Material *m, const Vector3f &n) {
        t = _t;
        material = m;
        normal = n;
    }

    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
    }

    // destructor
    ~Hit() = default;

    float getT() const {
        return t;
    }

    shared_ptr<Material> getMaterial() const { // Material *getMaterial() const {
        return material;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    void set(float _t, shared_ptr<Material> m, const Vector3f &n) {
    // void set(float _t, Material *m, const Vector3f &n) {
        t = _t;
        material = m;
        normal = n;
    }

    void set_face_normal(const Ray& r, const Vector3f& outward_normal); // remove inline

public:
    float t;
    shared_ptr<Material> material; // Material *material;
    Vector3f normal;
    Vector3f p; // ray point at parameter
    float u, v; // 纹理映射
    bool front_face;

};

/*
inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}
*/

#endif // HIT_H
