# MexIFaceExample - Main CMakeLists.txt
#
# A fully working example project using the MexIFace package with C++ and Matlab object-oriented APIs.
# Demonstrates use of Armadillo, BLAS LAPACK, and OpenMP.
#
# Mark J. Olah (mjo@cs.unm DOT edu)
# Copyright 2017-2019
# Licensed under the Apache License, Version 2.0
# https://www.apache.org/licenses/LICENSE-2.0
# See: LICENSE file

cmake_minimum_required( VERSION 3.9 )
project(MexIFaceExample VERSION 0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    option(BUILD_TESTING "Build testing framework" ON)
else()
    option(BUILD_TESTING "Build testing framework" OFF)
endif()
option(OPT_DOC "Build documentation" OFF)
option(OPT_INSTALL_TESTING "Install testing executables" OFF)
option(OPT_EXPORT_BUILD_TREE "Make package usable and exportable from the build tree.  Useful for development and debugging." OFF)
option(OPT_BLAS_INT64 "Use 64-bit integers for Armadillo, BLAS, and LAPACK. [Required if OPT_MATLAB is on]" OFF)
option(OPT_MATLAB "Build and install Matlab MEX modules and code" OFF)

if(OPT_MATLAB AND NOT OPT_BLAS_INT64)
    set(OPT_BLAS_INT64 True)
    set(OPT_BLAS_INT64 True CACHE BOOL "Use 64-bit integers for Armadillo, BLAS, and LAPACK. [Forced on by OPT_MATLAB]." FORCE)
endif()

message(STATUS "BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "OPTION: BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "OPTION: BUILD_STATIC_LIBS: ${BUILD_STATIC_LIBS}")
message(STATUS "OPTION: BUILD_TESTING: ${BUILD_TESTING}")
message(STATUS "OPTION: OPT_DOC: ${OPT_DOC}")
message(STATUS "OPTION: OPT_INSTALL_TESTING: ${OPT_INSTALL_TESTING}")
message(STATUS "OPTION: OPT_EXPORT_BUILD_TREE: ${OPT_EXPORT_BUILD_TREE}")
message(STATUS "OPTION: OPT_BLAS_INT64: ${OPT_BLAS_INT64}")
message(STATUS "OPTION: OPT_MATLAB: ${OPT_MATLAB}")

#Add cmake/UcommonCmakeModules subpreo to path.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake/UncommonCMakeModules)

### Dependencies
find_package(OpenMP REQUIRED)

## Armadillo BLAS & LAPACK
# These are detected by custom FindArmadillo.cmake, FindBLAS.cmake and FindLAPACK.cmake find-modules in /cmake/UncommonCMakeModules
# These provide cross-platform pkg-config support and COMPONENT support for 64-bit integer support.  See individual files for details.
#
# Projects that don't use BLAS or LAPACK can just use these two lines:
#   find_package(Armadillo REQUIRED COMPONENTS CXX11)
#   set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${ARMADILLO_PRIVATE_COMPILE_DEFINITIONS})
#
# The ARMADILLO_PRIVATE_COMPILE_DEFINITIONS flags are produced by FindArmadillo.cmake.
# They are meant to be applied to all local target builds, but may conflict with downstream projects that use different options.
# Since CMake doesn't have a concept of non-transitive INTERFACE_COMPILE_DEFINITIONS, they cannot be a part of the
# Armadillo::Armadillo imported interface target and must be added seperately as private compile definitions to each target.
# We use a root-directory-level COMPILE_DEFINITIONS property to avoid having to remember to add them to each target individually.
# See: http://arma.sourceforge.net/docs.html#config_hpp for a full list of compile definition options.
if(OPT_BLAS_INT64)
    set(BLAS_INT_COMPONENT BLAS_INT64)
else()
    set(BLAS_INT_COMPONENT BLAS_INT32)
endif()
find_package(BLAS REQUIRED COMPONENTS ${BLAS_INT_COMPONENT})
find_package(LAPACK REQUIRED COMPONENTS ${BLAS_INT_COMPONENT})
find_package(Armadillo REQUIRED COMPONENTS BLAS LAPACK CXX11 ${BLAS_INT_COMPONENT})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${ARMADILLO_PRIVATE_COMPILE_DEFINITIONS})


if(OPT_MATLAB)
    include(AddExternalDependency)
    set(MexIFaceURL https://github.com/markjolah/MexIFace.git CACHE STRING "URL of MexIFace library dependency")
    add_external_dependency(NAME MexIFace URL ${MexIFaceURL} SHARED ${EXTERNAL_DEPENDENCY_TESTING})
endif()

### Compile flags and debug build configurations
add_compile_options($<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-O0>) #No optimization when debugging
add_compile_options($<$<CONFIG:Debug>:-W -Wall -Wextra>) #Add warnings for debug configurations
set(CMAKE_DEBUG_POSTFIX ".debug" CACHE STRING "Debug file extension") #Set the global debug postfix for libraries and executables

### PackageConfig Exports from cmake/UncommonCMakeModules/ExportPackageWizzard.cmake
#setup build-tree and install-tree exports and PackageConfig files
#EXPORTED_FIND_MODULES are needed by Exported CMake config files for use when find_package() is called in a downstream project.
#Packages that don't need BLAS/LAPACK support only need to export the FindArmadillo.cmake module.
#The CMake exporting code can also be done entirely manually.  The export_package_wizzard() is just for convenience.
include(ExportPackageWizzard)
set(EXPORTED_FIND_MODULES cmake/UncommonCMakeModules/FindArmadillo.cmake
                          cmake/UncommonCMakeModules/FindBLAS.cmake
                          cmake/UncommonCMakeModules/FindLAPACK.cmake
                          cmake/UncommonCMakeModules/MakePkgConfigTarget.cmake)
export_package_wizzard(PROVIDED_COMPONENTS ${ARMADILLO_INT64_COMPONENT} #Propagate BLAS integer size dependency
                       FIND_MODULES ${EXPORTED_FIND_MODULES})

### Main C++ library and executables
add_subdirectory(src)

### C++ Testing
if(BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

### MexIFace MEX Modules
if(OPT_MATLAB)
    message(STATUS "*** Matlab Module Building Enabled ***")
    add_subdirectory(src/MexIFace)
    mexiface_configure_install() #Configure and install startup${PROJECT_NAME}.m, and install Matlab code directories.
endif()

### Documentation - Doxygen
if(OPT_DOC)
    add_subdirectory(doc)
endif()
