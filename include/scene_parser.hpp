#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <cassert>
#include <vecmath.h>
#include <iostream>
#include "utils.hpp"

class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
// class Plane;
class Triangle;
class Transform;
class TriangleMesh;
class texture;
class lambertian;
class yz_rect;
class xz_rect;
class xy_rect;
class box;
class diffuse_light;
class MovingSphere;
class isotropic;
class constant_medium;

#define MAX_PARSER_TOKEN_LENGTH 1024

class SceneParser {
public:

    SceneParser() = delete;
    SceneParser(const char *filename);

    ~SceneParser();

    Camera *getCamera() const {
        return camera;
    }

    Vector3f getBackgroundColor() const {
        return background_color;
    }

    int getNumLights() const {
        return num_lights;
    }

    shared_ptr<Light> getLight(int i) const {
    // Light *getLight(int i) const {
        assert(i >= 0 && i < num_lights);
        return lights[i];
    }

    int getNumMaterials() const {
        return num_materials;
    }

    shared_ptr<Material> getMaterial(int i) const {
    // Material *getMaterial(int i) const {
        // std::cout << "get material index: " << i << std::endl;
        assert(i >= 0 && i < num_materials);
        return materials[i];
    }

    shared_ptr<Group> getGroup() const {
    // Group *getGroup() const {
        return group;
    }

private:

    void parseFile();
    // void parsePerspectiveCamera();
    void parseCamera(); // new
    void parseBackground();
    void parseLights();
    // Light *parsePointLight();
    shared_ptr<Light> parsePointLight();
    // Light *parseDirectionalLight();
    shared_ptr<Light> parseDirectionalLight();
    void parseMaterials();
    // Material *parseMaterial();
    shared_ptr<Material> parseMaterial();
    shared_ptr<lambertian> parseLambertian();
    shared_ptr<isotropic> parseIsotropic();
    shared_ptr<diffuse_light> parseDiffuseLight();
    shared_ptr<texture> parseTexture(char*);

    // Object3D *parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
    shared_ptr<Object3D> parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
    shared_ptr<Group> parseGroup(); // Group *parseGroup();
    shared_ptr<constant_medium> parseConstantMedium();
    shared_ptr<Sphere> parseSphere(); // Sphere *parseSphere();
    shared_ptr<MovingSphere> parseMovingSphere();
    // Plane *parsePlane();
    shared_ptr<Triangle> parseTriangle(); // Triangle *parseTriangle();
    shared_ptr<TriangleMesh> parseTriangleMesh(); // Mesh *parseTriangleMeshs();
    shared_ptr<Transform> parseTransform(); // Transform *parseTransform();
    shared_ptr<yz_rect> parseYZRect();
    shared_ptr<xz_rect> parseXZRect();
    shared_ptr<xy_rect> parseXYRect();
    shared_ptr<box> parseBox();

    int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);

    Vector3f readVector3f();

    float readFloat();
    int readInt();

    FILE *file;
    Camera *camera;
    Vector3f background_color;
    int num_lights;
    // Light **lights;
    std::vector<shared_ptr<Light>> lights;
    int num_materials;
    // Material **materials;
    std::vector<shared_ptr<Material>> materials;
    // Material *current_material;
    shared_ptr<Material> current_material;
    // Group *group;
    shared_ptr<Group> group;
};

#endif // SCENE_PARSER_H
