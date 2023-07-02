cmake_minimum_required(VERSION 3.24)

include(FetchContent)
FetchContent_Declare(lodepng GIT_REPOSITORY "https://github.com/lvandeve/lodepng.git" GIT_TAG "master")

message(CHECK_START "Fetching lodepng from GitHub")
FetchContent_Populate(lodepng)
message(CHECK_PASS "Done")

#message(CHECK_START "Copying lodepng sources")
#file(COPY "lodepng.h" "lodepng.cpp" DESTINATION ${lodepng_SOURCE_DIR})
#message(CHECK_PASS "Done")

add_library(lodepng INTERFACE)

target_sources(lodepng INTERFACE
        $<BUILD_INTERFACE:${lodepng_SOURCE_DIR}/lodepng.cpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_DATAROOTDIR}/lodepng/lodepng.cpp>
        )

target_sources(lodepng INTERFACE FILE_SET HEADERS
        BASE_DIRS
        ${lodepng_source_dir}

        FILES
        ${lodepng_headers})

target_include_directories(lodepng INTERFACE
        $<BUILD_INTERFACE:${lodepng_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lodepng>
        )

target_link_libraries(lodepng INTERFACE ${CMAKE_DL_LIBS})