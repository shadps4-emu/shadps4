cmake_minimum_required(VERSION 3.22)

project(shadps4)

# Default to a Release build
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release." FORCE)
    message(STATUS "Defaulting to a Release build")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/hooks/pre-commit AND NOT EXISTS ${PROJECT_SOURCE_DIR}/.git/hooks/pre-commit)
    if (EXISTS ${PROJECT_SOURCE_DIR}/.git/)
        message(STATUS "Copying pre-commit hook")
        file(COPY hooks/pre-commit DESTINATION ${PROJECT_SOURCE_DIR}/.git/hooks)
    endif()
endif()

# Configure C++ standard
# ===========================

if (MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/std:c++20>)
else()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Output binaries to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)


# Setup a custom clang-format target (if clang-format can be found) that will run
# against all the src files. This should be used before making a pull request.
# =======================================================================

set(CLANG_FORMAT_POSTFIX "-15")
find_program(CLANG_FORMAT
    NAMES clang-format${CLANG_FORMAT_POSTFIX}
          clang-format
    PATHS ${PROJECT_BINARY_DIR}/externals)
# if find_program doesn't find it, try to download from externals
if (NOT CLANG_FORMAT)
    if (WIN32 AND NOT CMAKE_CROSSCOMPILING)
        message(STATUS "Clang format not found! Downloading...")
        set(CLANG_FORMAT "${PROJECT_BINARY_DIR}/externals/clang-format${CLANG_FORMAT_POSTFIX}.exe")
        file(DOWNLOAD
            https://github.com/yuzu-emu/ext-windows-bin/raw/master/clang-format${CLANG_FORMAT_POSTFIX}.exe
            "${CLANG_FORMAT}" SHOW_PROGRESS
            STATUS DOWNLOAD_SUCCESS)
        if (NOT DOWNLOAD_SUCCESS EQUAL 0)
            message(WARNING "Could not download clang format! Disabling the clang format target")
            file(REMOVE ${CLANG_FORMAT})
            unset(CLANG_FORMAT)
        endif()
    else()
        message(WARNING "Clang format not found! Disabling the clang format target")
    endif()
endif()

if (CLANG_FORMAT)
    set(SRCS ${PROJECT_SOURCE_DIR}/src)
    set(CCOMMENT "Running clang format against all the .h and .cpp files in src/")
    if (WIN32)
        add_custom_target(clang-format
            COMMAND powershell.exe -Command "Get-ChildItem '${SRCS}/*' -Include *.cpp,*.h -Recurse | Foreach {&'${CLANG_FORMAT}' -i $_.fullname}"
            COMMENT ${CCOMMENT})
    elseif(MINGW)
        add_custom_target(clang-format
            COMMAND find `cygpath -u ${SRCS}` -iname *.h -o -iname *.cpp | xargs `cygpath -u ${CLANG_FORMAT}` -i
            COMMENT ${CCOMMENT})
    else()
        add_custom_target(clang-format
            COMMAND find ${SRCS} -iname *.h -o -iname *.cpp | xargs ${CLANG_FORMAT} -i
            COMMENT ${CCOMMENT})
    endif()
    unset(SRCS)
    unset(CCOMMENT)
endif()

# Include source code
# ===================

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_target_directory_groups target_name)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    get_target_property(target_sources "${target_name}" SOURCES)

    foreach(file_name IN LISTS target_sources)
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

include_directories(src)

add_executable(shadps4
    src/main.cpp
)

create_target_directory_groups(shadps4)
