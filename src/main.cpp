#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include "renderer.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    // if (argc != 3) {
    //     // cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
    //     return 1;
    // }
    
    char* inputFile = argv[1];
    char* outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    
    // RayCasting
    // RayCaster ray_caster(inputFile, outputFile);
    // ray_caster.render();

    // RayTracing
    RayTracer ray_tracer(inputFile, outputFile, atoi(argv[3]), atoi(argv[4]));
    ray_tracer.render();

    cout << "\nHello! Computer Graphics!" << endl;
    return 0;
}

