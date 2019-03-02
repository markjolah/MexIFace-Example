# src/MexIFace/CMakeLists.txt
# Main CMakeLists.txt for ArmadilloExampleIFace MexIFace based wrapper class

file(GLOB SRCS *.cpp)

foreach(src IN LISTS SRCS)
    get_filename_component(target ${src} NAME_WE)
    mexiface_make_mex(MEXNAME ${target} SOURCES ${src} LINK_LIBRARIES ArmadilloExample::ArmadilloExample)
endforeach()
