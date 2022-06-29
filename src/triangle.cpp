#include "triangle.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "aabb.hpp"

bool Triangle::hit(const Ray& ray, double tmin, double t_max, Hit& hit) const {
	Vector3f P0 = vertices[0], P1 = vertices[1], P2 = vertices[2]; // 三角形顶点
	Vector3f R0 = ray.getOrigin(); // 光线起始点
	Vector3f Rd = ray.getDirection(); Rd.normalize(); // 光线单位方向向量
	Vector3f E1 = P0 - P1, E2 = P0 - P2, S = P0 - R0;
	Matrix3f denominator(Rd, E1, E2, true);
	Matrix3f t_(S, E1, E2, true);
	Matrix3f b_(Rd, S, E2, true);
	Matrix3f r_(Rd, E1, S, true);
	float det_denominator = denominator.determinant();
	float t = t_.determinant() / det_denominator, b = b_.determinant() / det_denominator, r = r_.determinant() / det_denominator;
	t = t / ray.getDirection().length(); // Transform
	
	if (t > 0 && 0 <= b && b <= 1 && 0 <= r && r <= 1 && b + r <= 1 && tmin < t && t < hit.getT()) {
		Vector3f n = normal;
		/*
		if (useVN) {
			Vector3f point = ray.pointAtParameter(t);
			Vector3f va = (vertices[0] - point), vb = (vertices[1] - point), vc = (vertices[2] - point);
			float ra = Vector3f::cross(vb, vc).length(),
				rb = Vector3f::cross(vc, va).length(),
				rc = Vector3f::cross(va, vb).length();
			n = (ra * vn[0] + rb * vn[1] + rc * vn[2]).normalized();
		}
		*/

		hit.set(t, material, n); // 是否要判断normal的朝向？
		return true;
	}
	return false;
}

bool Triangle::bounding_box(double time0, double time1, aabb& output_box) const {
	Vector3f minimum(std::numeric_limits<float>::max()),
				maximum(std::numeric_limits<float>::min());
	for (int i = 0; i < 3; i++) { // 3 points
		for (int j = 0; j < 3; j++) { // x, y, z
			if (vertices[i][j] < minimum[j]) minimum[j] = vertices[i][j];
			if (vertices[i][j] > maximum[j]) maximum[j] = vertices[i][j];
		}
	}
	output_box = aabb(minimum, maximum);
	return true;
}

bool Triangle::intersect( const Ray& ray,  Hit& hit , float tmin) {
    Vector3f P0 = vertices[0], P1 = vertices[1], P2 = vertices[2]; // 三角形顶点
    Vector3f R0 = ray.getOrigin(); // 光线起始点
    Vector3f Rd = ray.getDirection(); Rd.normalize(); // 光线单位方向向量
    Vector3f E1 = P0 - P1, E2 = P0 - P2, S = P0 - R0;
    Matrix3f denominator(Rd, E1, E2, true);
    Matrix3f t_(S, E1, E2, true);
    Matrix3f b_(Rd, S, E2, true);
    Matrix3f r_(Rd, E1, S, true);
    float det_denominator = denominator.determinant();
    float t = t_.determinant() / det_denominator, b = b_.determinant() / det_denominator, r = r_.determinant() / det_denominator;
    t = t / ray.getDirection().length(); // Transform
    if(t > 0 && 0 <= b && b <= 1 && 0 <= r && r <= 1 && b + r <= 1 && tmin < t && t < hit.getT()){
        hit.set(t, material, normal);
        return true;
    }
    return false;
}
