CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(PA1 CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(PA1_SOURCES
        src/image.cpp
        src/main.cpp
        src/triangle_mesh.cpp
        src/scene_parser.cpp
        src/renderer.cpp
        src/texture.cpp
        src/camera.cpp
        src/material.cpp
        src/triangle.cpp
        src/sphere.cpp
        src/aabb.cpp
        src/hit.cpp
        src/group.cpp
        src/transform.cpp
        src/bvh.cpp
        src/plane.cpp
        src/box.cpp
        src/constant_medium.cpp)

SET(PA1_INCLUDES
        include/camera.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/light.hpp
        include/material.hpp
        include/triangle_mesh.hpp
        include/object3d.hpp
        include/plane.hpp
        include/ray.hpp
        include/scene_parser.hpp
        include/sphere.hpp
        include/transform.hpp
        include/triangle.hpp
        include/renderer.hpp
        include/utils.hpp
        include/aabb.hpp
        include/texture.hpp
        include/perlin.hpp
        include/bvh.hpp
        include/stb_image.hpp
        include/box.hpp
        include/constant_medium.hpp
        )

SET(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -fopenmp -std=c++17")
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${PA1_SOURCES} ${PA1_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
