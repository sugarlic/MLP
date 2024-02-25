#
# Made by godison
# https://github.com/DisonGo
#
cmake_minimum_required(VERSION 3.19)
set(P_NAME MLP)
project(${P_NAME} VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${EXE_DIR})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CURRENT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIR})
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL Charts PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL Charts PrintSupport)
set(RES_NAME res)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${RES_NAME})
file(GLOB_RECURSE RESOURCE_FILES 
  ${RES_DIR}/*.bmp
  ${RES_DIR}/*.cl
  ${RES_DIR}/*.bin
  ${RES_DIR}/*.wt
  ${RES_DIR}/*.csv
)

add_subdirectory(Network)
add_subdirectory(Utils)
add_subdirectory(MLP)
add_subdirectory(easy_bmp)
add_subdirectory(allocators)
add_subdirectory(custom_plot)
add_subdirectory(tests)
