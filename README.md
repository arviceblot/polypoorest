polypoorest
===========

Create images using an image-order, ray tracing engine algorithm.

This project was originally created as a class assignment. Modifications to the
original submission include the addition of some C++11 features, the
replacement of vector math library with GLM, and anything listed under [New features](#new-features).

## Features
* Diffuse materials and specular highlights
* Glossy reflections
* Point and area lights
* Parallel rendering
* Super basic triangle rasterizer, limited support
* PNG file export
* Anti-aliasing with jittered sampling

### New features
* Render preview window

### Potential future features
* Object instancing (transforms)
* OBJ file support
* Spot and directional lights
* Textures

## Requirements ##
### Linux ###
Mainly tested on Ubuntu 16.04.
* Cross-platform Make (CMake) v2.8+
* GNU Make or equivalent
* GCC or an alternative C++11 compatible compiler
* Boost v1.53+
* SFML v2+
* OpenGL
* GLM
* png++

## Building ##
### Release ###
```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### Debugging ###
```shell
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
