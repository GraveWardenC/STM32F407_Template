cmake_minimum_required(VERSION 3.25)


# 编译链
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/ArmGcc-toolchain.cmake CACHE STRING "ARM GCC Toolchain File")

set(CMAKE_COLOR_MAKEFILE ON CACHE BOOL "Enable color in makefile")

set(CMAKE_PROJECT_NAME TEMPLATE)
project(${CMAKE_PROJECT_NAME} LANGUAGES C ASM)


# 全局编译选项
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


message("------Build App for target STM32F407VGT6------")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Package/Output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Package/Lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Segger/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/STM32_Base/inc)
include_directories(User/Include/CMSIS/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/STM32_Base/Drivers/ST)

# 子目录添加
add_subdirectory(stm32_Base)
add_subdirectory(Segger)
add_subdirectory(Main)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release)" FORCE)
endif()

message(STATUS "Project Name: ${CMAKE_PROJECT_NAME}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Toolchain File: ${CMAKE_TOOLCHAIN_FILE}")






