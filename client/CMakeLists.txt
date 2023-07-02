cmake_minimum_required(VERSION 3.25)
project(SandboxClient VERSION 0.0.1 DESCRIPTION "Sandbox Game Client" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(CMakeRC CONFIG REQUIRED)
#find_package(freetype-gl CONFIG REQUIRED)
find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
add_subdirectory(libs/lodepng)

include_directories(${CMAKE_SOURCE_DIR}/game)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS src/*.hpp src/*.h)
file(GLOB_RECURSE PROJECT_ASSETS assets/*)

cmrc_add_resource_library(sandbox-resources ALIAS sandbox::rc NAMESPACE sandbox WHENCE assets ${PROJECT_ASSETS})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ../resources/resource.rc)

target_include_directories(${PROJECT_NAME} PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE SandboxGame sandbox::rc glfw lodepng GLEW::GLEW unofficial::enet::enet portaudio_static)