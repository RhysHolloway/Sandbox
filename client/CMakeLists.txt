cmake_minimum_required(VERSION 3.25)
project(SandboxClient VERSION 0.0.1 DESCRIPTION "Sandbox Game Client" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libs)
add_library(lodepng ${CMAKE_SOURCE_DIR}/libs/lodepng.h ${CMAKE_SOURCE_DIR}/libs/lodepng.cpp)

include_directories(${CMAKE_SOURCE_DIR}/game)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB PROJECT_SOURCES src/*.cpp src/*/*.cpp src/*/*/*.cpp src/*/*/*/*.cpp)
file(GLOB PROJECT_HEADERS src/*.hpp src/*/*.hpp src/*/*/*.hpp src/*/*/*/*.hpp src/*.h src/*/*.h src/*/*/*.h src/*/*/*/*.h)

add_custom_target(copy_assets COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ../resources/resource.rc)

add_dependencies(${PROJECT_NAME} copy_assets)

#target_compile_options(Sandbox PRIVATE -DUNICODE -DENGINE_DLL)

target_link_libraries(${PROJECT_NAME} PRIVATE SandboxGame glfw GLEW::GLEW glm::glm lodepng unofficial::enet::enet portaudio_static)
#copy_required_dlls(Sandbox)