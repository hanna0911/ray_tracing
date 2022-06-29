#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, shared_ptr<Material> m) : Object3D(m) {
	// Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->vertices[0] = a; this->vertices[1] = b; this->vertices[2] = c;
		Vector3f P1P2 = b - a, P1P3 = c - a;
		this->normal = Vector3f::cross(P1P2, P1P3);
		useVN = false;
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override;
	
	Vector3f normal;
	Vector3f vertices[3];

	// added functions
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    
	void setVN(const Vector3f& an, const Vector3f& bn, const Vector3f& cn) {
		useVN = true;
		vn[0] = an; vn[1] = bn; vn[2] = cn;
		// vn[0] = an.normalized(); vn[1] = bn.normalized(); vn[2] = cn.normalized();
	}


protected:
	bool useVN;
	Vector3f vn[3];
};

#endif //TRIANGLE_H
