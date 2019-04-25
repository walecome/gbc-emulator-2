include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(emulator CXX)

## Macro for adding source files
macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

add_subdirectory(src/)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Copy rom files to build folder
file(COPY roms DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "-g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const -fdiagnostics-color=always")

## Force colored output (mostly needed for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()

add_executable(emulator ${SRCS})



target_compile_features(emulator PUBLIC cxx_std_17)

## Move compile_commands.json to project directory after make
add_custom_command(
        TARGET emulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_HOME_DIRECTORY}/compile_commands.json)