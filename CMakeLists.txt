include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(emulator CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(lib/glfw)

set(GLAD_LIBRARIES dl)

include_directories(src/
                    lib/argparse/
                    lib/glad/include/
                    lib/glfw/include/
                    lib/imgui)

file(GLOB LIB_SOURCES lib/glad/src/glad.c)
file(GLOB LIB_SOURCES lib/imgui/*.cpp)
file(GLOB PROJECT_HEADERS src/*.hh)
file(GLOB PROJECT_SOURCES src/*.cc)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Libraries" FILES ${LIB_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")


## Copy rom files to build folder
file(COPY roms DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "-g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const -fdiagnostics-color=always")

## Force colored output (mostly needed for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()

add_executable(emulator ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                        ${LIB_SOURCES})

## Add ncurses GUI lib
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} glfw ${CURSES_LIBRARIES} ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

target_compile_features(emulator PUBLIC cxx_std_17)

## Move compile_commands.json to project directory after make
add_custom_command(
        TARGET emulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_HOME_DIRECTORY}/compile_commands.json)