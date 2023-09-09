cmake_minimum_required(VERSION 3.15)
project(MiniEigenProj)

set(CMAKE_CXX_STANDARD 14)

add_executable(TriangularMatrix main.cpp)

find_package(Eigen3 REQUIRED NO_MODULE)

list(APPEND CMAKE_INCLUDE_PATH "/opt/intel/oneapi/mkl/latest/include")
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -march=native -mtune=native")

set(CMAKE_CXX_COMPILER_FLAGS "-Wall
                              -Wuninitialized
                              -Werror=return-type
                              -Wno-terminate")

include_directories(${EIGEN3_INCLUDE_DIR})

#target_link_libraries(MiniEigenProj PRIVATE Eigen3::Eigen)


