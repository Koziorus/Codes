
project(MN_matrices_cpp)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions()

add_executable(MN_matrices_cpp main.cpp matrix.h vector.h vector.cpp matrix.cpp plot_helper.cpp plot_helper.h)
