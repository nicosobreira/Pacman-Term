cmake_minimum_required(VERSION 3.0)
project(pacman C)

# https://github.com/tony/ncurses-example/blob/master/CMakeLists.txt
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")

find_package(Curses REQUIRED)
find_library(MATH_LIBRARY m)

include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} ${MATH_LIBRARY})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
