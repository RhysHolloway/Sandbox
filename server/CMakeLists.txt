cmake_minimum_required(VERSION 3.25)
project(SandboxServer VERSION 0.0.1 DESCRIPTION "${GAME_TITLE} Server" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(plog CONFIG REQUIRED)
find_package(toml11 CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/game) # SandboxGame
include_directories(${CMAKE_SOURCE_DIR}/libs/enet)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS src/*.h)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR}/resources/resource.rc)
target_link_libraries(${PROJECT_NAME} PRIVATE SandboxGame) # plog::plog toml11::toml11)

if(MSVC)
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS /OPT:REF)
endif()