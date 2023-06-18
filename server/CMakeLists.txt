cmake_minimum_required(VERSION 3.25)
project(SandboxServer VERSION 0.0.1 DESCRIPTION "${GAME_TITLE} Server Launcher" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(plog CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/game)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB PROJECT_SOURCES src/*.cpp)
file(GLOB PROJECT_HEADERS src/*.h)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR}/resources/resource.rc)

#target_compile_options(Sandbox PRIVATE -DUNICODE -DENGINE_DLL)

target_link_libraries(${PROJECT_NAME} PRIVATE SandboxGame plog::plog)
#copy_required_dlls(Sandbox)