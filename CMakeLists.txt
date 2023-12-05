cmake_minimum_required(VERSION 3.22)

project(shadps4)

# Default to a Release build
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release." FORCE)
    message(STATUS "Defaulting to a Release build")
endif()

aux_source_directory(src/ sources)

include_directories(src/)

add_executable(shadps4
    ${sources}
)