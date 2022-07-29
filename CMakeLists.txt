cmake_minimum_required(VERSION 3.20)
project(lua_bloom C)

set(CMAKE_C_STANDARD 11)

include_directories(F:/conda/Library/include ./dep ./dep/murmur2)
link_directories("F:\\conda\\Library\\lib" F:/conda/Library/bin)

file(GLOB SOURCES  ./dep/*.c ./dep/murmur2/*.c)
message(${SOURCES})

add_library(bloom SHARED src/lbloom.c ${SOURCES})
target_link_libraries(bloom lua)

set_target_properties(bloom PROPERTIES OUTPUT_NAME bloom)
