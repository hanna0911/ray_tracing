#ifndef MESH_H
#define MESH_H

#include <vector>
#include <vecmath.h>
#include "object3d.hpp"

class Triangle;
class bvh_node;

class Mesh : public Object3D {

public:
    Mesh(const char *filename, shared_ptr<Material> m);
    // Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    std::vector<Vector3f> v; // vertices
    std::vector<TriangleIndex> t, vnIndex; // vn
    std::vector<Triangle *> faces; // store all triangle faces for further intersection
    std::vector<Vector3f> vn; // input given normal 法向量
    std::vector<Vector3f> n; // compute normal (if none vn) 人工计算出法向量
    std::vector<Vector2f> vt; // vt: texture (unused)

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    // added functions
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;


private:

    // Normal can be used for light estimation
    void computeNormal();
    
    // BVH加速
    aabb *box; // AABB包围盒
    Vector3f box_minimum, box_maximum;
    // bvh_node *bvhtree; // BVH树
};

#endif
