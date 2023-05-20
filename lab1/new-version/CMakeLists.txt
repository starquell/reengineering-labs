cmake_minimum_required(VERSION 3.0.0)
project(new-benchmarker VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include(CTest)
enable_testing()


add_executable(new-benchmarker main.cpp pugixml/src/pugixml.cpp)
target_include_directories(new-benchmarker PRIVATE pugixml/src)
target_link_libraries(new-benchmarker PRIVATE nlohmann_json::nlohmann_json)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
include(CPack)
