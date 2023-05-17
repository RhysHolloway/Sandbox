cmake_minimum_required(VERSION 3.25)
project(Sandbox VERSION 0.0.1 DESCRIPTION "Sandbox Game" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB PROJECT_SOURCES src/*.cpp src/*/*.cpp src/*/*/*.cpp)
file(GLOB PROJECT_HEADERS src/*.hpp src/*/*.hpp src/*/*/*.hpp src/*.h src/*/*.h src/*/*/*.h)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

#target_compile_options(Sandbox PRIVATE -DUNICODE -DENGINE_DLL)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW glm::glm opengl32)
#copy_required_dlls(Sandbox)