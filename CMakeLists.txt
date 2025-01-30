cmake_minimum_required(VERSION 3.22.1)
project(pacman C)

# https://github.com/tony/ncurses-example/blob/master/CMakeLists.txt
# -fsanitize=address
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c99 -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2")

find_package(Curses REQUIRED)
find_library(MATH_LIBRARY m)

include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

add_executable(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PRIVATE )
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} ${MATH_LIBRARY})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})

# https://github.com/meemknight/learnCmake/blob/master/moreCompleteExample/Cmakelists.txt
# This code creates a macro storing the current directory folder
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/assets")
# Uncomment next line and comment above for release build
# target_compile_definitions(${PROJECT_NAME} PRIVATE WORK_FOLDER="./assets")
