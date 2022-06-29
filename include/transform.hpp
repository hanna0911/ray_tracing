#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"

class Hit;
class Ray;

// transforms a 3D point using a matrix, returning a 3D point
static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
    return (mat * Vector4f(point, 1)).xyz();
}

// transform a 3D directino using a matrix, returning a direction
static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
    return (mat * Vector4f(dir, 0)).xyz();
}

// TODO: implement this class so that the intersect function first transforms the ray
class Transform : public Object3D {
public:
    Transform() {}

    Transform(const Matrix4f &m, shared_ptr<Object3D> obj) : o(obj) {
    // Transform(const Matrix4f &m, Object3D *obj) : o(obj) {
        transform = m.inverse();
    }

    ~Transform() {
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin);

    // added functions
    bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;
    bool bounding_box(double time0, double time1, aabb& output_box) const;

protected:
    // Object3D *o; //un-transformed object
    shared_ptr<Object3D> o;
    Matrix4f transform;
};



#endif //TRANSFORM_H
