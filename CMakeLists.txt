cmake_minimum_required(VERSION 3.25)
project(Sandbox VERSION 0.0.1 DESCRIPTION "Sandbox Game" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(GAME_TITLE Sandboxington)

add_subdirectory(game)
add_subdirectory(server)
add_subdirectory(client)