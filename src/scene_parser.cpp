#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "scene_parser.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "object3d.hpp"
#include "group.hpp"
#include "triangle_mesh.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "transform.hpp"
#include "texture.hpp"
#include "box.hpp"

#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

SceneParser::SceneParser(const char *filename) {

    // initialize some reasonable default values
    // group = nullptr;
    camera = nullptr;
    background_color = Vector3f(0.5, 0.5, 0.5);
    num_lights = 0;
    // lights = nullptr;
    num_materials = 0;
    // materials = nullptr;
    current_material = nullptr;

    // parse the file
    assert(filename != nullptr);
    const char *ext = &filename[strlen(filename) - 4];

    if (strcmp(ext, ".txt") != 0) {
        printf("wrong file name extension\n");
        exit(0);
    }
    file = fopen(filename, "r");

    if (file == nullptr) {
        printf("cannot open scene file\n");
        exit(0);
    }
    parseFile();
    fclose(file);
    file = nullptr;

    if (num_lights == 0) {
        printf("WARNING:    No lights specified\n");
    }
}

SceneParser::~SceneParser() {

    // delete group;
    delete camera;

    int i;
    /*
    for (i = 0; i < num_materials; i++) {
        delete materials[i];
    }
    delete[] materials;
    */
    /*
    for (i = 0; i < num_lights; i++) {
        delete lights[i];
    }
    delete[] lights;
    */
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
    //
    // at the top level, the scene can have a camera, 
    // background color and a group of objects
    // (we add lights and other things in future assignments)
    //
    char token[MAX_PARSER_TOKEN_LENGTH];
    while (getToken(token)) {
        // if (!strcmp(token, "PerspectiveCamera")) {
        //     parsePerspectiveCamera();
        if (!strcmp(token, "Camera")) {
            parseCamera();
        } else if (!strcmp(token, "Background")) {
            parseBackground();
        } else if (!strcmp(token, "Lights")) {
            parseLights();
        } else if (!strcmp(token, "Materials")) {
            parseMaterials();
        } else if (!strcmp(token, "Group")) {
            group = parseGroup();
        } else {
            printf("Unknown token in parseFile: '%s'\n", token);
            exit(0);
        }
    }
}

// ====================================================================
// ====================================================================

/*
void SceneParser::parsePerspectiveCamera() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the camera parameters
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token);
    assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token);
    assert (!strcmp(token, "up"));
    Vector3f up = readVector3f();
    getToken(token);
    assert (!strcmp(token, "angle"));
    float angle_degrees = readFloat();
    float angle_radians = DegreesToRadians(angle_degrees);
    getToken(token);
    assert (!strcmp(token, "width"));
    int width = readInt();
    getToken(token);
    assert (!strcmp(token, "height"));
    int height = readInt();
    getToken(token);
    assert (!strcmp(token, "}"));
    camera = new PerspectiveCamera(center, direction, up, width, height, angle_radians);
}
*/

void SceneParser::parseCamera() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the camera parameters
    int width;
    int height;
    Vector3f lookfrom;
    Vector3f lookat;
    Vector3f vup;
    double vfov; // vertical field-of-view in degrees
    double aperture = 0;
    double focus_dist; 
    double time0 = 0;
    double time1 = 0;

    getToken(token);
    assert (!strcmp(token, "{"));
    
    while (true) {
        getToken(token);
        if (!strcmp(token, "}")) {
            break;
        } else if (!strcmp(token, "width")) {
            width = readInt();
        } else if (!strcmp(token, "height")) {
            height = readInt();
        } else if (!strcmp(token, "lookfrom")) {
            lookfrom = readVector3f();
        } else if (!strcmp(token, "lookat")) {
            lookat = readVector3f();
        } else if (!strcmp(token, "vup")) {
            vup = readVector3f();
        } else if (!strcmp(token, "vfov")) {
            vfov = readFloat();
        } else if (!strcmp(token, "focus_dist")) {
            focus_dist = readFloat();
        } else if (!strcmp(token, "aperture")) {
            aperture = readFloat();
        } else if (!strcmp(token, "time0")) {
            time0 = readFloat();
        } else if (!strcmp(token, "time1")) {
            time1 = readFloat();
        } else {
            printf("Unknown token in parseCamera: '%s'\n", token);
            assert(0);
        }
    }
    camera = new Camera(width, height, lookfrom, lookat, vup, vfov, (double)width / (double)height, aperture, focus_dist, time0, time1);
}


void SceneParser::parseBackground() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the background color
    getToken(token);
    assert (!strcmp(token, "{"));
    while (true) {
        getToken(token);
        if (!strcmp(token, "}")) {
            break;
        } else if (!strcmp(token, "color")) {
            background_color = readVector3f();
        } else {
            printf("Unknown token in parseBackground: '%s'\n", token);
            assert(0);
        }
    }
}

// ====================================================================
// ====================================================================

void SceneParser::parseLights() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    // read in the number of objects
    getToken(token);
    assert (!strcmp(token, "numLights"));
    num_lights = readInt();
    // lights = new Light *[num_lights];
    lights.resize(num_lights);
    // read in the objects
    int count = 0;
    while (num_lights > count) {
        getToken(token);
        if (strcmp(token, "DirectionalLight") == 0) {
            lights[count] = parseDirectionalLight();
        } else if (strcmp(token, "PointLight") == 0) {
            lights[count] = parsePointLight();
        } else {
            printf("Unknown token in parseLight: '%s'\n", token);
            exit(0);
        }
        count++;
    }
    getToken(token);
    assert (!strcmp(token, "}"));
}

shared_ptr<Light> SceneParser::parseDirectionalLight() {
// Light *SceneParser::parseDirectionalLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token);
    assert (!strcmp(token, "color"));
    Vector3f color = readVector3f();
    getToken(token);
    assert (!strcmp(token, "}"));
    return make_shared<DirectionalLight>(direction, color);
    // return new DirectionalLight(direction, color);
}

shared_ptr<Light> SceneParser::parsePointLight() {
// Light *SceneParser::parsePointLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "position"));
    Vector3f position = readVector3f();
    getToken(token);
    assert (!strcmp(token, "color"));
    Vector3f color = readVector3f();
    getToken(token);
    assert (!strcmp(token, "}"));
    // return new PointLight(position, color);
    return make_shared<PointLight>(position, color);
}
// ====================================================================
// ====================================================================


void SceneParser::parseMaterials() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    // read in the number of objects
    getToken(token);
    assert (!strcmp(token, "numMaterials"));
    num_materials = readInt();
    materials.resize(num_materials);
    // read in the objects
    int count = 0;
    while (num_materials > count) {
        getToken(token);
        if (!strcmp(token, "Material") ||
            !strcmp(token, "PhongMaterial")) {
            materials[count] = parseMaterial();
        } else if (!strcmp(token, "lambertian")) {
            materials[count] = parseLambertian();
        } else if (!strcmp(token, "diffuse_light")) {
            materials[count] = parseDiffuseLight();
        } else {
            printf("Unknown token in parseMaterial: '%s'\n", token);
            exit(0);
        }
        count++;
    }
    getToken(token);
    assert (!strcmp(token, "}"));
    std::cout << "finished parse materials" << std::endl;
}

shared_ptr<diffuse_light> SceneParser::parseDiffuseLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token); // texture token
    if (!strcmp(token, "customize")) {
        Vector3f custom = readVector3f();
        getToken(token);
        assert (!strcmp(token, "}"));
        return make_shared<diffuse_light>(custom);
    }
    shared_ptr<texture> text = parseTexture(token);
    getToken(token);
    assert (!strcmp(token, "}"));
    return make_shared<diffuse_light>(text);
}

shared_ptr<lambertian> SceneParser::parseLambertian() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token); // texture token
    if (!strcmp(token, "customize")) {
        Vector3f custom = readVector3f();
        getToken(token);
        assert (!strcmp(token, "}"));
        return make_shared<lambertian>(custom);
    }
    shared_ptr<texture> text = parseTexture(token);
    getToken(token);
    assert (!strcmp(token, "}"));
    return make_shared<lambertian>(text);
}

shared_ptr<texture> SceneParser::parseTexture(char* token) {
    shared_ptr<texture> text;
    while (true) {
        if (!strcmp(token, "checker")) {
            getToken(token);
            assert(!strcmp(token, "{"));
            Vector3f even = Vector3f::ZERO;
            Vector3f odd = Vector3f::ZERO;
            while (true) {
                getToken(token);
                if (!strcmp(token, "}")) {
                    break;
                } else if (!strcmp(token, "even")) {
                    even = readVector3f();
                } else if (!strcmp(token, "odd")) {
                    odd = readVector3f();
                } else {
                    printf("Unknown token in cheker: '%s'\n", token);
                    exit(0);
                }
            }
            text = make_shared<checker_texture>(even, odd);
            std::cout << "parsed checker texture: " << even.x() << " " << even.y() << " " << even.z() << ", " << odd.x() << " " << odd.y() << " " << odd.z() << std::endl;
            return text;
        } else if (!strcmp(token, "solid")) {
            
        } else if (!strcmp(token, "noise")) {
            
        } else if (!strcmp(token, "image")) {
            char filename[MAX_PARSER_TOKEN_LENGTH];
            getToken(filename);
            text = make_shared<image_texture>(filename);
            return text;
        } else {
            printf("Unknown token in texture: '%s'\n", token);
            exit(0);
        }
        getToken(token);
    }
    return text; // can be deleted
}

shared_ptr<Material> SceneParser::parseMaterial() {
// Material *SceneParser::parseMaterial() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    char filename[MAX_PARSER_TOKEN_LENGTH];
    filename[0] = 0;
    Vector3f diffuseColor(1, 1, 1), specularColor(0, 0, 0);
    float shininess = 0;
    getToken(token);

    assert (!strcmp(token, "{"));
    while (true) {
        getToken(token);
        if (strcmp(token, "diffuseColor") == 0) {
            diffuseColor = readVector3f();
        } else if (strcmp(token, "specularColor") == 0) {
            specularColor = readVector3f();
        } else if (strcmp(token, "shininess") == 0) {
            shininess = readFloat();
        } else {
            assert (!strcmp(token, "}"));
            break;
        }
    }
    shared_ptr<Material> answer = make_shared<Material>(diffuseColor, specularColor, shininess);
    return answer;
}


// ====================================================================
// ====================================================================

shared_ptr<Object3D> SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) {
// Object3D *SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) {
    // Object3D *answer = nullptr;
    std::cout << ", token: " << token << std::endl;
    shared_ptr<Object3D> answer;
    if (!strcmp(token, "Group")) {
        // answer = (Object3D *) parseGroup();
        answer = parseGroup();
    } else if (!strcmp(token, "Sphere")) {
        // answer = (Object3D *) parseSphere();
       answer = parseSphere();
    } /*else if (!strcmp(token, "Plane")) {
        answer = (Object3D *) parsePlane();
    } */else if (!strcmp(token, "Triangle")) {
        // answer = (Object3D *) parseTriangle();
        answer = parseTriangle();
    } else if (!strcmp(token, "TriangleMesh")) {
        // answer = (Object3D *) parseTriangleMesh();
        answer = parseTriangleMesh();
        std::cout << "finished parsing parseTriangleMesh" << std::endl;
    } else if (!strcmp(token, "Transform")) {
        // answer = (Object3D *) parseTransform();
        answer = parseTransform();
    } else if (!strcmp(token, "yz_rect")) {
        answer = parseYZRect();
    } else if (!strcmp(token, "xz_rect")) {
        answer = parseXZRect();
    } else if (!strcmp(token, "xy_rect")) {
        answer = parseXYRect();
    } else if (!strcmp(token, "box")) {
        answer = parseBox();
        std::cout << "finished parsing box" << std::endl;
    } else {
        printf("Unknown token in parseObject: '%s'\n", token);
        exit(0);
    }
    std::cout << "finish parsing objects" << std::endl;
    return answer;
}

// ====================================================================
// ====================================================================

shared_ptr<Group> SceneParser::parseGroup() {
// Group *SceneParser::parseGroup() {
    //
    // each group starts with an integer that specifies
    // the number of objects in the group
    //
    // the material index sets the material of all objects which follow,
    // until the next material index (scoping for the materials is very
    // simple, and essentially ignores any tree hierarchy)
    //
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));

    // read in the number of objects
    getToken(token);
    assert (!strcmp(token, "numObjects"));
    int num_objects = readInt();
    std::cout << "numobjects of group: " << num_objects << std::endl;

    // auto *answer = new Group(num_objects);
    shared_ptr<Group> answer = make_shared<Group>(num_objects);

    // read in the objects
    int count = 0;
    while (num_objects > count) {
        getToken(token);
        if (!strcmp(token, "MaterialIndex")) {
            // change the current material
            int index = readInt();
            assert (index >= 0 && index <= getNumMaterials());
            current_material = getMaterial(index);
        } else {
            // Object3D *object = parseObject(token);
            std::cout << "get object index: " << count;
            shared_ptr<Object3D> object = parseObject(token);
            assert (object != nullptr);
            // answer->addObject(count, object);
            answer->add(object);

            count++;
        }
    }
    getToken(token);
    assert (!strcmp(token, "}"));

    // return the group
    std::cout << "finished parse group" << std::endl;
    return answer;
}

// ====================================================================
// ====================================================================

shared_ptr<Sphere> SceneParser::parseSphere() {
// Sphere *SceneParser::parseSphere() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token);
    assert (!strcmp(token, "radius"));
    float radius = readFloat();
    getToken(token);
    assert (!strcmp(token, "}"));
    assert (current_material != nullptr);
    // return new Sphere(center, radius, current_material);
    return make_shared<Sphere>(center, radius, current_material);
}

/*
Plane *SceneParser::parsePlane() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "normal"));
    Vector3f normal = readVector3f();
    getToken(token);
    assert (!strcmp(token, "offset"));
    float offset = readFloat();
    getToken(token);
    assert (!strcmp(token, "}"));
    assert (current_material != nullptr);
    return new Plane(normal, offset, current_material);
}
*/

shared_ptr<Triangle> SceneParser::parseTriangle() {
// Triangle *SceneParser::parseTriangle() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "vertex0"));
    Vector3f v0 = readVector3f();
    getToken(token);
    assert (!strcmp(token, "vertex1"));
    Vector3f v1 = readVector3f();
    getToken(token);
    assert (!strcmp(token, "vertex2"));
    Vector3f v2 = readVector3f();
    getToken(token);
    assert (!strcmp(token, "}"));
    assert (current_material != nullptr);
    // return new Triangle(v0, v1, v2, current_material);
    return make_shared<Triangle>(v0, v1, v2, current_material);
}

shared_ptr<TriangleMesh> SceneParser::parseTriangleMesh() {
// Mesh *SceneParser::parseTriangleMesh() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    char filename[MAX_PARSER_TOKEN_LENGTH];
    // get the filename
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "obj_file"));
    getToken(filename);
    getToken(token);
    assert (!strcmp(token, "}"));
    const char *ext = &filename[strlen(filename) - 4];
    assert(!strcmp(ext, ".obj"));
    // Mesh *answer = new Mesh(filename, current_material);
    std::cout << "create mesh obj" << std::endl;
    // shared_ptr<Mesh> answer = make_shared<Mesh>(filename, current_material);
    shared_ptr<TriangleMesh> answer = make_shared<TriangleMesh>(filename, current_material);
    std::cout << "finish create mesh obj" << std::endl;
    return answer;
}

shared_ptr<box> SceneParser::parseBox() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    Vector3f min, max;
    while (true) {
        getToken(token);
        if (!strcmp(token, "}")) {
            break;
        } else if (!strcmp(token, "min")) {
            min = readVector3f();
            std::cout << "min: " << min.x() << " " << min.y() << " " << min.z() << std::endl;
        } else if (!strcmp(token, "max")) {
            max = readVector3f();
            std::cout << "max: " << max.x() << " " << max.y() << " " << max.z() << std::endl;
        } else {
            printf("Unknown token in parseBox: '%s'\n", token);
            exit(0);
        }
    }
    std::cout << "exit from here? " << std::endl;
    shared_ptr<box> answer = make_shared<box>(min, max, current_material);
    std::cout << "created box answer" << std::endl;
    return answer;
}

shared_ptr<xy_rect> SceneParser::parseXYRect() {
    float x0, x1, y0, y1, k;
    x0 = readFloat();
    x1 = readFloat();
    y0 = readFloat();
    y1 = readFloat();
    k = readFloat();
    return make_shared<xy_rect>(x0, x1, y0, y1, k, current_material);
}

shared_ptr<xz_rect> SceneParser::parseXZRect() {
    float x0, x1, y0, y1, k;
    x0 = readFloat();
    x1 = readFloat();
    y0 = readFloat();
    y1 = readFloat();
    k = readFloat();
    return make_shared<xz_rect>(x0, x1, y0, y1, k, current_material);
}

shared_ptr<yz_rect> SceneParser::parseYZRect() {
    float x0, x1, y0, y1, k;
    x0 = readFloat();
    x1 = readFloat();
    y0 = readFloat();
    y1 = readFloat();
    k = readFloat();
    return make_shared<yz_rect>(x0, x1, y0, y1, k, current_material);
}


shared_ptr<Transform> SceneParser::parseTransform() {
// Transform *SceneParser::parseTransform() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    Matrix4f matrix = Matrix4f::identity();
    // Object3D *object = nullptr;
    shared_ptr<Object3D> object;
    getToken(token);
    assert (!strcmp(token, "{"));
    // read in transformations: 
    // apply to the LEFT side of the current matrix (so the first
    // transform in the list is the last applied to the object)
    getToken(token);

    while (true) {
        if (!strcmp(token, "Scale")) {
            Vector3f s = readVector3f();
            matrix = matrix * Matrix4f::scaling(s[0], s[1], s[2]);
        } else if (!strcmp(token, "UniformScale")) {
            float s = readFloat();
            matrix = matrix * Matrix4f::uniformScaling(s);
        } else if (!strcmp(token, "Translate")) {
            matrix = matrix * Matrix4f::translation(readVector3f());
        } else if (!strcmp(token, "XRotate")) {
            matrix = matrix * Matrix4f::rotateX(DegreesToRadians(readFloat()));
        } else if (!strcmp(token, "YRotate")) {
            matrix = matrix * Matrix4f::rotateY(DegreesToRadians(readFloat()));
        } else if (!strcmp(token, "ZRotate")) {
            matrix = matrix * Matrix4f::rotateZ(DegreesToRadians(readFloat()));
        } else if (!strcmp(token, "Rotate")) {
            getToken(token);
            assert (!strcmp(token, "{"));
            Vector3f axis = readVector3f();
            float degrees = readFloat();
            float radians = DegreesToRadians(degrees);
            matrix = matrix * Matrix4f::rotation(axis, radians);
            getToken(token);
            assert (!strcmp(token, "}"));
        } else if (!strcmp(token, "Matrix4f")) {
            Matrix4f matrix2 = Matrix4f::identity();
            getToken(token);
            assert (!strcmp(token, "{"));
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    float v = readFloat();
                    matrix2(i, j) = v;
                }
            }
            getToken(token);
            assert (!strcmp(token, "}"));
            matrix = matrix2 * matrix;
        } else {
            // otherwise this must be an object,
            // and there are no more transformations
            object = parseObject(token);
            break;
        }
        getToken(token);
    }

    assert(object != nullptr);
    getToken(token);
    assert (!strcmp(token, "}"));
    // return new Transform(matrix, object);
    return make_shared<Transform>(matrix, object);
}


// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
    // for simplicity, tokens must be separated by whitespace
    assert (file != nullptr);
    int success = fscanf(file, "%s ", token);
    if (success == EOF) {
        token[0] = '\0';
        return 0;
    }
    return 1;
}


Vector3f SceneParser::readVector3f() {
    float x, y, z;
    int count = fscanf(file, "%f %f %f", &x, &y, &z);
    if (count != 3) {
        printf("Error trying to read 3 floats to make a Vector3f\n");
        assert (0);
    }
    return Vector3f(x, y, z);
}


float SceneParser::readFloat() {
    float answer;
    int count = fscanf(file, "%f", &answer);
    if (count != 1) {
        printf("Error trying to read 1 float\n");
        assert (0);
    }
    return answer;
}


int SceneParser::readInt() {
    int answer;
    int count = fscanf(file, "%d", &answer);
    if (count != 1) {
        printf("Error trying to read 1 int\n");
        assert (0);
    }
    return answer;
}
