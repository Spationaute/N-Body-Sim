#Set the minimum cmake version required
cmake_minimum_required(VERSION 3.5)
project(NBody)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(SOURCE_FILES src/main.cpp src/Settings.cpp src/Hyperphysics.cpp src/Hyperphysics.h)
find_package(Armadillo REQUIRED)
find_package(CUDAToolkit)
find_package(OpenMP REQUIRED)
find_package(jsoncpp REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
add_definitions(-DAMRA_DONT_USE_WRAPPER)
add_executable(NBody ${SOURCE_FILES})
include_directories(ARMADILLO_INCLUDE_DIR)
include_directories(jsoncpp_INCLUDE_DIRS)

if(CUDAToolkit_FOUND)
    message("Adding NVBLAS for GPU computation")
    include_directories(CUDAToolkit_INCLUDE_DIRS)
    target_link_libraries(NBody  PUBLIC
        armadillo
        jsoncpp
        nvblas
        blas
        lapack
    )
else()
    target_link_libraries(NBody PUBLIC
        armadillo
        jsoncpp
    )
endif()

