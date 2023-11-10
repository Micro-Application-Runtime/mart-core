cmake_minimum_required(VERSION 3.10)

# Project name
project(mart C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Include the ExternalProject module
include(ExternalProject)

# Set QuickJS directory
set(QUICKJS_DIR ${PROJECT_SOURCE_DIR}/3rd/quickjs)

# Add QuickJS as an external project
add_custom_target(build_quickjs
    COMMAND ${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${QUICKJS_DIR}
    COMMENT "build quickjs"
    add_custom_target ${QUICKJS_DIR}/libquickjs.a
)


# Set libuv directory
set(LIBUV_DIR ${PROJECT_SOURCE_DIR}/3rd/libuv)

# Add libuv subdirectory
add_subdirectory(${LIBUV_DIR} EXCLUDE_FROM_ALL)

# Set binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set source files
file(GLOB_RECURSE SOURCE_FILES "src/*.c")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} build_quickjs)

# Add QuickJS library
add_library(quickjs STATIC IMPORTED)
set_target_properties(quickjs PROPERTIES IMPORTED_LOCATION ${QUICKJS_DIR}/libquickjs.a)
include_directories(${QUICKJS_DIR})

# Link libuv to the executable
target_link_libraries(${PROJECT_NAME} uv_a quickjs dl m)
