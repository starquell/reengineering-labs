cmake_minimum_required(VERSION 3.0.0)
project(old-benchmarker VERSION 0.1.0)

include(CTest)
enable_testing()
add_executable(old-benchmarker main.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
