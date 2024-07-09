cmake_minimum_required(VERSION 3.28)
project(untitled C)

set(CMAKE_C_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(
        untitled src/main/main.c
        src/http/server.c
        include/server.h
        src/log.c
        include/log.h
        src/http/request.c
        include/request.h
        src/http/response.c
        include/strings.h
        src/parsing/strings.c
        include/response.h
        src/parsing/url.c
        include/url.h
        src/http/dispatch.c
        include/dispatch.h
        src/http/controller.c
        include/controller.h
        src/vector.c
        include/vector.h
        src/http/media.c
        include/media.h
        src/map/intmap.c
        include/intmap.h
        src/map/stringmap.c
        include/stringmap.h
)
if(WIN32)
    target_link_libraries(untitled ws2_32)
endif()
