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

set(CMAKE_CXX_FLAGS "-g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const")

add_executable(emulator ${SRCS})
target_compile_features(emulator PUBLIC cxx_std_17)

## Copy compile_commands.json to project directory
file(COPY ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_HOME_DIRECTORY})

