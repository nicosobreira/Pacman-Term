cmake_minimum_required(VERSION 3.0)
project(pacman C)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
