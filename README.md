Ray Tracer
==========

Create images using an image-order, ray tracing engine algorithm.

This project was originally created as a class assignment. Modifications to the
original submission include the addition of some C++11 features and the
replacement of vector math library with GLM.

## Requirements ##
### Linux ###
* Cross-platform Make (CMake) v2.8+
* GNU Make or equivalent.
* GCC or an alternative C++11 compatible compiler.
* Boost v1.53+
* SFML v2+
* OpenGL
* GLM
* png++

## Building ##
### Release ###
```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

### Debugging ###
```
$ mkdir debug
$ cd debug
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
```
