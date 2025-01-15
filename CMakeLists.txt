cmake_minimum_required(VERSION 3.16)

project(DinosTakeOverTheWorld C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "Don't build raylib examples")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build raylib as a static library")

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 4.5.0
  SOURCE_DIR ./third_party/raylib
)

FetchContent_MakeAvailable(raylib)

file(GLOB SOURCES ./source/*.c)

list(REMOVE_ITEM SOURCES ./source/main.c)

add_executable(${PROJECT_NAME} ./source/main.c ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
