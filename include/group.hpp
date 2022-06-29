#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
// #include "ray.hpp"
// #include "hit.hpp"
#include <iostream>
#include <vector>
// #include "aabb.hpp"

class Hit;
class aabb;
class Ray;

// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:

    Group() {
        this->capacity = 0;
    }

    explicit Group (int num_objects) {
        this->capacity = num_objects;
    }

    ~Group() override {

    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool True = false;
        for(int i = 0; i < capacity; i++){
            if(data[i]->intersect(r, h, tmin)) True = true;
        }
        if(True) return true;
        return false;
    }

    void addObject(int index, Object3D *obj) {
        data.push_back(obj);
    }

    int getGroupSize() {
        return capacity;
    }
    
    // added functions
    Group(shared_ptr<Object3D> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Object3D> object) { objects.push_back(object); }

    virtual bool hit(
        const Ray& r, double t_min, double t_max, Hit& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public: 
    std::vector<shared_ptr<Object3D>> objects; // added

private:
    std::vector<Object3D*> data;
    int capacity;

};


#endif
	
