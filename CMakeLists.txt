cmake_minimum_required(VERSION 3.28)
project(untitled C)

set(CMAKE_C_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES
        src/*.c
        src/http/*.c
        src/http/request/*.c
        src/parsing/*.c
        src/map/*.c
        src/vector.c
        src/http/common/content-type.c
        src/http/media.c
        third-party/cJSON.c
        third-party/cJSON_Utils.c
)

file(GLOB HEADERS
        include/*.h
        include/http/*.h
        include/http/request/*.h
        include/parsing/*.h
        include/map/*.h
        include/vector.h
        include/http/media.h
        include/iterable.h
        third-party/cJSON.h
        third-party/cJSON_Utils.h
)

add_executable(untitled ${SOURCES} ${HEADERS}
        src/main/main.c)

if(WIN32)
    target_link_libraries(untitled ws2_32)
endif()