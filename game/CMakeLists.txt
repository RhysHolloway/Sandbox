cmake_minimum_required(VERSION 3.25)
project(SandboxGame VERSION 0.0.1 DESCRIPTION "${GAME_TITLE} Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
#find_package(plog CONFIG REQUIRED)
#find_package(Tracy CONFIG REQUIRED)

find_package(toml11 REQUIRED)
find_package(CMakeRC CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libs/FastNoiseLite)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/sandbox")
file(GLOB_RECURSE PROJECT_SOURCES sandbox/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS sandbox/*.hpp sandbox/*.h)

file(GLOB_RECURSE SANDBOX_ASSETS assets/*)
cmrc_add_resource_library(sandbox-resources ALIAS sandbox::rc NAMESPACE sandbox WHENCE assets ${SANDBOX_ASSETS})

add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC sandbox::rc glm::glm toml11::toml11) # Tracy::TracyClient