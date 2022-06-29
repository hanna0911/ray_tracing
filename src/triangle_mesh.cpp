#include "triangle_mesh.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>
#include "triangle.hpp"
#include "bvh.hpp"
#include "aabb.hpp"
#include "utils.hpp"
#include "group.hpp"

bool TriangleMesh::intersect(const Ray &r, Hit &h, float tmin) {

    // Optional: Change this brute force method into a faster one.
    bool result = false;
    for (int triId = 0; triId < (int) t.size(); ++triId) {
        TriangleIndex triIndex = t[triId];
        Triangle triangle(v[triIndex[0]],
                          v[triIndex[1]], v[triIndex[2]], material);
        triangle.normal = n[triId];
        result |= triangle.intersect(r, h, tmin);
    }
    return result;
}


TriangleMesh::TriangleMesh(const char *filename, shared_ptr<Material> material) : Object3D(material) {
// Mesh::Mesh(const char *filename, Material *material) : Object3D(material) {
    // std::cout << "constructing Mesh obj" << std::endl;
    // Optional: Use tiny obj loader to replace this simple one.
    std::ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        std::cout << "Cannot open " << filename << "\n";
        return;
    }
    std::string line;
    std::string vTok("v"); // Vertices
    std::string fTok("f"); // Faces: 1000
    std::string texTok("vt"); // ?? didn't save, texture?
    std::string normalTok("vn"); // TODO: normal
    char bslash = '/', space = ' ';
    std::string tok;
    int texID; // ?? no use
    
    box_minimum = Vector3f(std::numeric_limits<float>::max());
    box_maximum = Vector3f(std::numeric_limits<float>::min());
    bool input_vn = false; // 是否输入vn这一量

    while (true) {
        // if(input_vn) std::cout << "input_vn" << std::endl;
        std::getline(f, line);
        if (f.eof()) {
            break;
        }
        if (line.size() < 3) {
            continue;
        }
        if (line.at(0) == '#') {
            continue;
        }
        std::stringstream ss(line);
        ss >> tok;
        if (tok == vTok) { // "v" -> v
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            // 更新aabb 大bug!!!
            for (int j = 0; j < 3; j++) { // x, y, z
                if (vec[j] < box_minimum[j]) box_minimum[j] = vec[j];
                if (vec[j] > box_maximum[j]) box_maximum[j] = vec[j];
            }
            v.push_back(vec);
        } else if (tok == fTok) { // "f" -> t
            TriangleIndex vId, nId;
            for (int i = 0; i < 3; ++i) {
                std::string str;
                ss >> str;
                
                std::string::size_type pos;
                std::vector<std::string> id;
                str += "/";
                for (int i = 0; i < str.size(); i++) {
                    pos = str.find("/", i);
                    if (pos < str.size()) {
                        std::string s = str.substr(i, pos - i);
                        id.push_back(s);
                        i = pos;
                    }
                }

                vId[i] = atoi(id[0].c_str()) - 1;
                if (id.size() > 2) {
                    nId[i] = atoi(id[2].c_str()) - 1;
                }
            }
            t.push_back(vId);
            vnIndex.push_back(nId);
        } else if (tok == texTok) { // "vt" (unused)
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
        } else if (tok == normalTok) { // "vn" -> n
            input_vn = true;
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            vn.push_back(vec);
        }
    }
    if(!input_vn) computeNormal(); // 如果没有输入normal，则计算normal

    f.close();

    sides = make_shared<Group>(); // important!

    // std::cout << "start construct aabb box for mesh obj" << std::endl;

    // aabb
    // box = make_shared<aabb>(box_minimum, box_minimum);   

    for (int triId = 0; triId < (int) t.size(); ++triId) {
        TriangleIndex& triIndex = t[triId];
        TriangleIndex& normalIndex = vnIndex[triId];
        shared_ptr<Triangle> triangle = make_shared<Triangle>(v[triIndex[0]], 
            v[triIndex[1]], v[triIndex[2]], material);
        if (input_vn) triangle->setVN(vn[normalIndex[0]], 
            vn[normalIndex[1]], vn[normalIndex[2]]);
        else triangle->normal = n[triId];
        // faces.push_back(triangle);
        sides->add(triangle);
    }


    // build tree
    // bvhtree = new bvh_node(faces, 0, faces.size(), 0, 0);

    // std::cout << "finish constructing mesh obj" << std::endl;

}


void TriangleMesh::computeNormal() {
    n.resize(t.size());
    for (int triId = 0; triId < (int) t.size(); ++triId) {
        TriangleIndex& triIndex = t[triId];
        Vector3f a = v[triIndex[1]] - v[triIndex[0]];
        Vector3f b = v[triIndex[2]] - v[triIndex[0]];
        b = Vector3f::cross(a, b);
        n[triId] = b / b.length();
    }
}


bool TriangleMesh::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
    return sides->hit(r, t_min, t_max, rec);
}

bool TriangleMesh::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(box_minimum, box_maximum);
    return true;
}
