cmake_minimum_required(VERSION 3.25)
project(SandboxClient VERSION 0.0.1 DESCRIPTION "Sandbox Game Client" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(CMakeRC CONFIG REQUIRED)
find_path(ENET_DIR ../libs/enet/enet.h)
find_path(BTP_INCLUDE "BS_thread_pool.hpp")
add_subdirectory(libs/lodepng)

#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
#    set(portaudio_type portaudio)
#ELSEIF(CMAKE_BUILD_TYPE MATCHES RELEASE)
#    set(portaudio_type portaudio_static)
#ENDIF (CMAKE_BUILD_TYPE MATCHES DEBUG)

include_directories(${CMAKE_SOURCE_DIR}/game) # SandboxGame
include_directories(${CMAKE_SOURCE_DIR}/libs/enet) # enet

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS src/*.hpp src/*.h)

file(GLOB_RECURSE CLIENT_ASSETS assets/*)
cmrc_add_resource_library(client-resources ALIAS client::rc NAMESPACE client WHENCE assets ${CLIENT_ASSETS})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${BTP_INCLUDE} {ENET_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE SandboxGame client::rc glfw lodepng GLEW::GLEW ${portaudio_type})

if(MSVC)
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS /OPT:REF)
endif()