cmake_minimum_required(VERSION 3.25)

# project root dir 
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message("------Build App for target STM32F407VGT6------")

# toolchain file for arm gcc STM32F407VGT6
set(CMAKE_TOOLCHAIN_FILE ${PROJECT_ROOT_DIR}/Cmake/ArmGcc-toolchain.cmake CACHE STRING "ARM GCC Toolchain File")

set(CMAKE_COLOR_MAKEFILE ON CACHE BOOL "Enable color in makefile")

set(CMAKE_PROJECT_NAME TEMPLATE)
project(${CMAKE_PROJECT_NAME} LANGUAGES C ASM)


# C  Standard == C11 and enable C Extensions
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Package/Output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Package/Lib)


add_subdirectory(Project)

# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release)" FORCE)
# endif()

# message(STATUS "Project Name: ${CMAKE_PROJECT_NAME}")
# message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
# message(STATUS "Toolchain File: ${CMAKE_TOOLCHAIN_FILE}")






