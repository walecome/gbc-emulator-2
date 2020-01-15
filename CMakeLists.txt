include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(emulator C CXX)

include_directories(src/
                    lib/argparse/
                    lib/glad/include/
                    lib/imgui/)

file(GLOB PROJECT_HEADERS src/*.hh)
file(GLOB PROJECT_SOURCES src/*.cc)
file(GLOB IMGUI_SOURCES lib/imgui/*.cpp)


message("imgui sources: ${IMGUI_SOURCES}")

source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Imgui sources" FILES ${IMGUI_SOURCES})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

set(CMAKE_CXX_FLAGS "-g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -fdiagnostics-color=always")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
# target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_LIBRARIES dl)
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
# target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# imgui
set(IMGUI_DIR "${LIB_DIR}/imgui")
add_library("imgui" ${IMGUI_SOURCES})
target_include_directories("imgui" PRIVATE "${IMGUI_DIR}/")
target_include_directories(${PROJECT_NAME} PRIVATE "${IMGUI_DIR}/")
target_link_libraries(${PROJECT_NAME} "imgui")

target_link_libraries(${PROJECT_NAME} "glfw" "glad" "imgui")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
## Move compile_commands.json to project directory after make
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_HOME_DIRECTORY}/compile_commands.json)

## Force colored output (for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()