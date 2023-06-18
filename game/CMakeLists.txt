cmake_minimum_required(VERSION 3.25)
project(SandboxGame VERSION 0.0.1 DESCRIPTION "${GAME_TITLE} Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(toml11 CONFIG REQUIRED)
#find_package(plog CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)

include_directories(../libs)
add_library(FastNoiseLite ../libs/FastNoiseLite.h ../libs/FastNoiseLite.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/sandbox")

file(GLOB PROJECT_SOURCES sandbox/*.cpp sandbox/*/*.cpp sandbox/*/*/*.cpp sandbox/*/*/*/*.cpp)
file(GLOB PROJECT_HEADERS sandbox/*.hpp sandbox/*/*.hpp sandbox/*/*/*.hpp sandbox/*/*/*/*.hpp sandbox/*.h sandbox/*/*.h sandbox/*/*/*.h sandbox/*/*/*/*.h)

add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} GLEW::GLEW glm::glm FastNoiseLite toml11::toml11 unofficial::enet::enet)