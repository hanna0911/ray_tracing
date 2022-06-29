#ifndef RENDERER_H
#define RENDERER_H

#include "utils.hpp"

class SceneParser;
class Ray;
class Vector3f;
class Group;

class Renderer {
public:
    Renderer(char* _scene, char* _output);
    virtual void render() = 0;
protected:
    SceneParser *scene;
    char* output;
};

class RayCaster : public Renderer {
public:
    RayCaster(char* _scene, char* _output) : Renderer(_scene, _output) {}
    void render();
protected:
    Vector3f traceRay(const Ray &camRay);
};

class RayTracer : public Renderer {
public:
    RayTracer(char* _scene, char* _output, int _samples = 100, int _depth = 30) : Renderer(_scene, _output),
        samples(_samples), depth(_depth) {}
    void render();
protected:
    int samples; // samples_per_pixel
    int depth; // max_depth
    Vector3f traceRay(const Ray& r, int depth, const Vector3f &background, shared_ptr<Group> world);
    // Vector3f traceRay(const Ray& r, int depth, const Vector3f &background, const Group world);
};

#endif