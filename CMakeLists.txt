cmake_minimum_required(VERSION 3.15.0)

if ($ENV{EXPORT_COMPILE_COMMANDS})
    # ccls needs compile_commands.json to work
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${ProjectId} C)

set(CMAKE_C_STANDARD 11)

# file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.c")
set(PROJECT_SOURCE_FILES $ENV{SOURCE})
if (WIN32)
	# to unix path
	string(REPLACE "\\" "/" PROJECT_SOURCE_FILES "${PROJECT_SOURCE_FILES}")
endif()
# https://stackoverflow.com/a/22148384/20935957
separate_arguments(PROJECT_SOURCE_FILES)
# note: if you want to change main target name (aka executable name)
# make sure you also change it in run.sh and run.bat
# set(MAIN_TARGET ${PROJECT_NAME})
set(MAIN_TARGET app)
add_executable(${MAIN_TARGET} ${PROJECT_SOURCE_FILES})

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${MAIN_TARGET} PRIVATE -Wall -Wextra -Wpedantic -O3 $<$<CONFIG:Debug>:-pg>)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${MAIN_TARGET} PRIVATE -Wall -Wextra -Wpedantic -O3 $<$<CONFIG:Debug>:-pg>)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${MAIN_TARGET} PRIVATE /W4 /utf-8 $<$<CONFIG:Release>:/O2> $<$<CONFIG:Debug>:-pg>)
    target_compile_definitions(${MAIN_TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if ($ENV{COLOR_DIAGNOSTICS})
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${MAIN_TARGET} PRIVATE -fcolor-diagnostics)
    elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${MAIN_TARGET} PRIVATE -fdiagnostics-color=always)
    elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        # undocumented; untested
        target_compile_options(${MAIN_TARGET} PRIVATE /diagnostics:color)
    endif()
endif()

# target_link_libraries(${MAIN_TARGET} PRIVATE)
target_include_directories(${MAIN_TARGET} PRIVATE "include/")

