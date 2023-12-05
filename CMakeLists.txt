cmake_minimum_required(VERSION 2.8.0)

project(shadps4)

aux_source_directory(src/ sources)

include_directories(src/)

add_executable(shadps4
    ${sources}
)