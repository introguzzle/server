cmake_minimum_required(VERSION 3.28)
project(untitled C)

set(CMAKE_C_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(
        untitled src/main.c
        src/server.c
        include/server.h
        src/log.c
        include/log.h
        src/request.c
        include/request.h
        src/response.c
        include/parsing.h
        src/parsing.c
        include/response.h
        src/map.c
        include/map.h
)
if(WIN32)
    target_link_libraries(untitled ws2_32)
endif()
