cmake_minimum_required(VERSION 3.24)

include(FetchContent)

FETCHCONTENT_DECLARE(FastNoiseLite
        URL https://raw.githubusercontent.com/Auburn/FastNoiseLite/master/Cpp/FastNoiseLite.h
#        URL_HASh SHA256=
        DOWNLOAD_NO_EXTRACT true
        TLS_VERIFY true
    )

message(CHECK_START "Fetching FastNoiseLite from GitHub")
FETCHCONTENT_POPULATE(FastNoiseLite)
message(CHECK_PASS "Done")

add_library(FastNoiseLite INTERFACE)

target_sources(FastNoiseLite INTERFACE FILE_SET HEADERS BASE_DIRS ${FASTNOISELITE_SOURCE_DIR} FILES ${FASTNOISELITE_HEADERS})

target_link_libraries(FastNoiseLite INTERFACE ${CMAKE_DL_LIBS})