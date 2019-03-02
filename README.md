# MexIFaceExample-Armadillo

This is a fully functional example project using the [MexIFace](http://markjolah.github.io/MexIFace) C++/Matlab interface.

Example project features:
 * Builds an independent C++ library with no Matlab dependencies unless `OPT_MATLAB=On`
 * Demonstrates the proper BLAS and LAPACK integration with Matlab using 65-bit integers.
 * Demonstrates the use of OpenMP in MEX modules
 * Provides full [CMake Packaging](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#cmake-packages-7) support with the ability to export the package from the build or install trees.
 * Provides support for debugging and release builds
 * C++ unit testing using [googletest](https://github.com/google/googletest)
 * Matlab testing using [function-based unit tests](https://www.mathworks.com/help/matlab/matlab_prog/write-function-based-unit-tests-.html)
 * Full [Doxygen](http://www.doxygen.nl/) documentation system with support for HTML and PDF outputs
 * Continuous integration testing using TravisCI
 * Automatic Github Pages documentation update with TravisCI
 * Cross-compilation support from Linux to matlab `glnxa64` and `win64` target arch environments.

## Background

[MexIFace](https://github.com/markjolah/MexIFace) is a C++/Matlab object-based interface library and cross-platform CMake build system for creating interactive Matlab MEX modules with persistent state and complex behavior.  This library provides a fully functional example of how flexible and extensible C++/Matlab numerical applications can be developed using the MexIFace framework.  Most of the boiler-plate build system and dependency resolution logic has been integrated into the MexIFace package or the bundled UncommonCMakeModules git subrepo.

References:
* [MexIFace documentation](https://markjolah.github.io/MexIFace)
* [UncommonCMakeModules github](https://github.com/markjolah/UncommonCMakeModules) - CMake modules and toolchains to simplify resolving dependencies, cross-compiling, linking, installing, and exporting of complex MEX modules.
* [ci-numerical-dependencies github](https://github.com/markjolah/ci-numerical-dependecies) - Scripts for installing Armadillo/BLAS/LAPACk with 64-bit integer support on TravisCI/ubuntu systems.


## Building and Installation

### C++-Only library and testing builds

### Matlab Linux

### Matlab Windows
