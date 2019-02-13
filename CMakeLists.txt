# MexIFace-ArmadilloExample - Main CMakeLists.txt
#
# An example of using the MexIFace  C++ and Matlab interface with Armadillo and OpenMP.
#

cmake_minimum_required( VERSION 3.9 )
project(ArmadilloExample VERSION 0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    option(BUILD_TESTING "Build testing framework" ON)
else()
    option(BUILD_TESTING "Build testing framework" OFF)
endif()
option(OPT_INSTALL_TESTING "Install testing executables" OFF)
option(OPT_EXPORT_BUILD_TREE "Configure the package so it is usable from the build tree.  Useful for development." OFF)
option(OPT_ARMADILLO_INT64 "Use 64-bit integers for Armadillo, BLAS, and LAPACK. [Must be on if OPT_MATLAB is on]" OFF)
option(OPT_MATLAB "Build and install matlab mex modules and code" OFF)

if(OPT_MATLAB AND NOT OPT_ARMADILLO_INT64)
    set(OPT_ARMADILLO_INT64 True)
    set(OPT_ARMADILLO_INT64 True CACHE BOOL "Use 64-bit integers for Armadillo, BLAS, and LAPACK. [Forced on by OPT_MATLAB]." FORCE)
endif()

message(STATUS "OPTION: BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "OPTION: BUILD_STATIC_LIBS: ${BUILD_STATIC_LIBS}")
message(STATUS "OPTION: BUILD_TESTING: ${BUILD_TESTING}")
message(STATUS "OPTION: OPT_INSTALL_TESTING: ${OPT_INSTALL_TESTING}")
message(STATUS "OPTION: OPT_EXPORT_BUILD_TREE: ${OPT_EXPORT_BUILD_TREE}")
message(STATUS "OPTION: OPT_ARMADILLO_INT64: ${OPT_ARMADILLO_INT64}")
message(STATUS "OPTION: OPT_MATLAB: ${OPT_MATLAB}")

#Add cmake/UcommonCmakeModules subpreo to path.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake/UncommonCMakeModules)

### Dependencies
if(OPT_MATLAB)
    include(AddExternalDependency)
    set(MexIFaceURL https://github.com/markjolah/MexIFace.git CACHE STRING "URL of MexIFace library dependency")
    add_external_dependency(NAME MexIFace URL ${MexIFaceURL} SHARED ${EXTERNAL_DEPENDENCY_TESTING})
endif()

#Armadillo
if(OPT_ARMADILLO_INT64)
    set(ARMADILLO_INT64_COMPONENT INT64) #Enable INT64 interface for Armadillo, BLAS and LAPACK
else()
    set(ARMADILLO_INT64_COMPONENT)
endif()
find_package(BLAS REQUIRED COMPONENTS ${ARMADILLO_INT64_COMPONENT})
find_package(LAPACK REQUIRED COMPONENTS ${ARMADILLO_INT64_COMPONENT})
find_package(Armadillo REQUIRED COMPONENTS CXX11 ${ARMADILLO_INT64_COMPONENT})
add_compile_definitions(${ARMADILLO_PRIVATE_COMPILE_DEFINITIONS})

find_package(OpenMP REQUIRED)

include(ConfigureDebugBuilds) #setup debug build configurations

### PackageConfig Exports from UncommonCMakeModules/ExportPackageWizzard.cmake
#setup build-tree and install-tree exports and PackageConfig files
include(ExportPackageWizzard)
#EXPORTED_FIND_MODULES are needed by Exported CMake config file for use when find_package() is called downstream
set(EXPORTED_FIND_MODULES FindArmadillo.cmake FindBLAS.cmake FindLAPACK.cmake MakePkgConfigTarget.cmake)
list(TRANSFORM EXPORTED_FIND_MODULES PREPEND cmake/UncommonCMakeModules/)
export_package_wizzard(PROVIDED_COMPONENTS ${ARMADILLO_INT64_COMPONENT}
                       FIND_MODULES ${EXPORTED_FIND_MODULES})

add_subdirectory(src) #library source

if(BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

if(OPT_MATLAB)
    message(STATUS "*** Matlab Module Building Enabled ***")
    add_subdirectory(src/MexIFace)
    mexiface_configure_install() #Matlab code and startup<PROJECT_NAME>.m configure and install
endif()
