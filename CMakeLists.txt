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
set(CMAKE_C_FLAGS "-g")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rd/flashdb/inc)
include_directories(${PROJECT_SOURCE_DIR}/3rd/flashdb/port/fal/inc)
include_directories(${PROJECT_SOURCE_DIR}/src/config)

# Set source files
file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/src/*.c")

include_directories(
    ${PROJECT_SOURCE_DIR}/3rd/EasyLogger/easylogger/inc
    # ${PROJECT_SOURCE_DIR}/3rd/EasyLogger/easylogger/plugins/file
    ${PROJECT_SOURCE_DIR}/src/platform/linux/easylogger
)

file(GLOB_RECURSE EASY_LOGGER_SOURCE
    "${PROJECT_SOURCE_DIR}/3rd/EasyLogger/easylogger/src/*.c"
    "${PROJECT_SOURCE_DIR}/3rd/EasyLogger/easylogger/src/platform/linux/easylogger/*c")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE} ${EASY_LOGGER_SOURCE})

add_dependencies(${PROJECT_NAME} build_quickjs)

# Add QuickJS library
add_library(quickjs STATIC IMPORTED)
set_target_properties(quickjs PROPERTIES IMPORTED_LOCATION ${QUICKJS_DIR}/libquickjs.a)
include_directories(${QUICKJS_DIR})

# Link libuv to the executable
target_link_libraries(${PROJECT_NAME} uv_a quickjs dl m)
