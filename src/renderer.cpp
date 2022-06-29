#include "renderer.hpp"
#include "utils.hpp"
#include "sphere.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "scene_parser.hpp"
#include "texture.hpp"
#include "ray.hpp"
#include "hit.hpp"

// ====================================================================
// Ray Casting
// ====================================================================

Renderer::Renderer(char* _scene, char* _output) {
    // scene = new SceneParser(_scene);
    output = _output;
}

void RayCaster::render() {
    /*
    Camera* camera = scene->getCamera();
    Image renderedImg(camera->getWidth(), camera->getHeight());
    // 循环屏幕空间的像素
    time_t start = time(NULL);
#pragma omp parallel for schedule(dynamic, 1)        
    for (int x = 0; x < camera->getWidth(); ++x) {
        float elapsed = (time(NULL) - start), progress = (float)(1.0 + x) / (float)camera->getWidth();
        fprintf(stderr, "\rRendering %5.2f%% Time: %.2f/%.2f sec", progress * 100., elapsed, elapsed / progress);
        for (int y = 0; y < camera->getHeight(); ++y) {
            // 计算当前像素(x,y)处相机出射光线camRay
            Ray camRay = camera->generateRay(Vector2f(x, y)); 
            Hit hit;
            Vector3f finalColor = traceRay(camRay);
            renderedImg.SetPixel(x, y, finalColor);
        }
    }
    renderedImg.SaveImage(output);
    */
}

Vector3f RayCaster::traceRay(const Ray &camRay) {
    /*
    // 判断camRay是否和场景有交点，并返回最近交点的数据，存储在hit中
    Group* baseGroup = scene->getGroup();
    Hit hit;
    bool isIntersect = baseGroup->intersect(camRay, hit, 0.0001);
    if (isIntersect) {
        Vector3f finalColor = Vector3f::ZERO;
        // 找到交点之后，累加来自所有光源的光强影响
        for (int li = 0; li < scene->getNumLights(); ++li) {
            Light* light = scene->getLight(li);
            Vector3f L, lightColor;
            // 获得光照强度
            light->getIllumination(camRay.pointAtParameter(hit.getT()), L, lightColor); // 计算局部光强
            finalColor += hit.getMaterial()->Shade(camRay, hit, L, lightColor);
        }
        return finalColor;
    } else {
    // 不存在交点，返回背景色
        return scene->getBackgroundColor();
    }
    */
}

// ====================================================================
// Ray Tracing
// ====================================================================


void RayTracer::render() {

    /*
    Camera *cam = scene->getCamera();
    int image_width = cam->getWidth();
    int image_height = cam->getHeight();
    Image image(image_width, image_height);
    */
    int image_width = 400;
    auto aspect_ratio = 16.0 / 9.0; 

    // 暂时强行改变scene =================================
    Group objects;
    auto checker = make_shared<checker_texture>(Vector3f(0.2, 0.3, 0.1), Vector3f(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Vector3f(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<Sphere>(Vector3f(0, 10, 0), 10, make_shared<lambertian>(checker)));

    // =================================================
    

    // 暂时强行改变camera ================================
    Vector3f background = Vector3f(0.70, 0.80, 1.00);
    Vector3f lookfrom = Vector3f(13,2,3);
    Vector3f lookat = Vector3f(0,0,0);
    auto vfov = 20.0;
    auto aperture = 0.0;
    Vector3f vup(0,1,0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    Camera *cam = new Camera(image_width, image_height, lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    Image image(image_width, image_height);

    // =================================================

    // Anti-aliasing

    // Jitter Sampling: High Res
    int widthHighRes = image_width * 3;
    int heightHighRes = image_height * 3;
    std::vector<Vector3f> pixColors;
    std::vector<Vector3f> pixColorsBlurH;
    std::vector<Vector3f> pixColorsBlurV;
    const float K[5] = {0.1201f, 0.2339f, 0.2931f, 0.2339f, 0.1201f};
    
    time_t start = time(NULL);

    /*
    pixColors.resize(widthHighRes * heightHighRes);
    for (int i = 0; i < widthHighRes; i++) {
        float elapsed = (time(NULL) - start), progress = (1. + i) / widthHighRes;
        fprintf(stderr, "\rRendering %5.2f%% Time: %.2f/%.2f sec", progress * 100., elapsed, elapsed / progress);
#pragma omp parallel for schedule(dynamic, 1)
        for (int j = 0; j < heightHighRes; j++) {
            // Antialiasing: Multiple Samples
            Vector3f origColor = Vector3f::ZERO;
            for (int s = 0; s < samples; ++s) {
                Vector2f coord = Vector2f((2 * float(i) / (widthHighRes - 1)) - 1, 
                                          (2 * float(j) / (heightHighRes - 1)) - 1);
                float r_i = (float)rand() / (float)RAND_MAX - 0.5f,
                      r_j = (float)rand() / (float)RAND_MAX - 0.5f;
                Vector2f newcoord = Vector2f(coord[0] + r_i / (widthHighRes - 1),
                                             coord[1] + r_j / (heightHighRes - 1));
                Ray r = cam->generateRandomRay(newcoord);
                origColor += traceRay(r, depth, background, objects);
            }
            pixColors[i * widthHighRes + j] = origColor;
        }
    }

    // Gaussian Filter: High Res horizontal blur
    for (int i = 0; i < widthHighRes; i++) {
        for (int j = 0; j < heightHighRes; j++){
            if (j == 0) {
                pixColorsBlurH.push_back(pixColors[i * heightHighRes] * K[2] + 
                                         pixColors[i * heightHighRes + 1] * K[3] + 
                                         pixColors[i * heightHighRes + 2] * K[4]); 
            } else if (j == 1) {
                pixColorsBlurH.push_back(pixColors[i * heightHighRes] * K[1] +
                                         pixColors[i * heightHighRes + 1] * K[2] + 
                                         pixColors[i * heightHighRes + 2] * K[3] + 
                                         pixColors[i * heightHighRes + 3] * K[4]);
            } else if (j == heightHighRes - 1) {
                pixColorsBlurH.push_back(pixColors[i * heightHighRes + j] * K[2] + 
                                         pixColors[i * heightHighRes + j - 1] * K[1] + 
                                         pixColors[i * heightHighRes + j - 2] * K[0]);   
            } else if (j == heightHighRes - 2) {
                pixColorsBlurH.push_back(pixColors[i * heightHighRes + j + 1] * K[3] +
                                         pixColors[i * heightHighRes + j] * K[2] + 
                                         pixColors[i * heightHighRes + j - 1] * K[1] + 
                                         pixColors[i * heightHighRes + j - 2] * K[0]);   
            }
            else{
                pixColorsBlurH.push_back(pixColors[i * heightHighRes + j - 2] * K[0] +
                                         pixColors[i * heightHighRes + j - 1] * K[1] +
                                         pixColors[i * heightHighRes + j] * K[2] + 
                                         pixColors[i * heightHighRes + j + 1] * K[3] + 
                                         pixColors[i * heightHighRes + j + 2] * K[4]);   
            }
        }
    }

    // Gaussian Filter: High Res vertical blur
    for (int j = 0; j < heightHighRes; j++) {
        for (int i = 0; i < widthHighRes; i++) {
            if (i == 0) {
                pixColorsBlurV.push_back(pixColorsBlurH[j * widthHighRes] * K[2] +
                                         pixColorsBlurH[j * widthHighRes + 1] * K[3] +
                                         pixColorsBlurH[j * widthHighRes + 2] * K[4]);
            }
            else if (i == 1) {
                pixColorsBlurV.push_back(pixColorsBlurH[j * widthHighRes ] * K[1] +
                                         pixColorsBlurH[j * widthHighRes + 1] * K[2] +
                                         pixColorsBlurH[j * widthHighRes + 2] * K[3] +
                                         pixColorsBlurH[j * widthHighRes + 3] * K[4]);
            } else if (i == widthHighRes - 1) {
                pixColorsBlurV.push_back(pixColorsBlurH[j * widthHighRes + i] * K[2] +
                                         pixColorsBlurH[j * widthHighRes + i - 1] * K[1] +
                                         pixColorsBlurH[j * widthHighRes + i - 2] * K[0]);
            } else if (i == widthHighRes - 2) {
                pixColorsBlurV.push_back(pixColorsBlurH[j * widthHighRes + i + 1] * K[3] +
                                         pixColorsBlurH[j * widthHighRes + i] * K[2] +
                                         pixColorsBlurH[j * widthHighRes + i - 1] * K[1] +
                                         pixColorsBlurH[j * widthHighRes + i - 2] * K[0]);
            } else {
                pixColorsBlurV.push_back(pixColorsBlurH[j * widthHighRes + i - 2] * K[0] +
                                         pixColorsBlurH[j * widthHighRes + i - 1] * K[1] +
                                         pixColorsBlurH[j * widthHighRes + i] * K[2] +
                                         pixColorsBlurH[j * widthHighRes + i + 1] * K[3] +
                                         pixColorsBlurH[j * widthHighRes + i + 2] * K[4]);
            }
        }
    }

    // DownSample
    for (int i = 0; i < image_width; i++) {
        for (int j = 0; j < image_height; j++) {
            Vector3f finalPixCol = pixColorsBlurV[3 * (i + widthHighRes * j) + 0] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + 1] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + 2] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + widthHighRes + 0] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + widthHighRes + 1] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + widthHighRes + 2] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + 2 * widthHighRes + 0] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + 2 * widthHighRes + 1] +
                                   pixColorsBlurV[3 * (i + widthHighRes * j) + 2 * widthHighRes + 2];
            Vector3f finalPixColAVG = finalPixCol / 9.0f;
            image.SetPixel(j, i, finalPixColAVG);
        }
    }
    // END SOLN
    */



#pragma omp parallel for schedule(dynamic, 1)
    for (int j = image_height-1; j >= 0; --j) {
        float elapsed = (time(NULL) - start), progress = (float)(image_height - j) / (float)image_height;
        fprintf(stderr, "\rRendering %5.2f%% Time: %.2f/%.2f sec", progress * 100., elapsed, elapsed / progress);
        for (int i = 0; i < image_width; ++i) {
            Vector3f pixel_color(0, 0, 0);
            for (int s = 0; s < samples; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                Ray r = cam->generateRandomRay(Vector2f(u, v));
                pixel_color += traceRay(r, depth, background, objects) / (float)samples;
            }
            image.SetPixel(i, j, pixel_color);
            // write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    // save the file
    image.SaveImage(output);
}

Vector3f RayTracer::traceRay(const Ray& r, int depth, const Vector3f &background, const Group& world) {
    // Vector3f background = scene->getBackgroundColor();
    // Group *world = scene->getGroup();
    Hit rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Vector3f::ZERO;

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    Ray scattered;
    Vector3f attenuation;
    Vector3f emitted = rec.material->emitted(rec.u, rec.v, rec.p);

    if (!rec.material->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * traceRay(scattered, depth - 1, background, world);
}



// ====================================================================
// Path Tracing
// ====================================================================


// ====================================================================
// Photon Mapping
// ====================================================================