cmake_minimum_required(VERSION 3.16)
project(lab6 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(lab6 src/main.c include/image_parser.h include/structs.h src/image_parser.c include/image_manip.h src/image_manip.c)

target_link_libraries(lab6 m)
