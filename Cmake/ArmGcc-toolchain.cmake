# 指定构建目标
set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# specify cross-compilers and tools
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    if(DEFINED ENV{ARM_CROSS_BUILD_GENERIC_PATH})
        message("ARM_CROSS_BUILD_PATH: $ENV{ARM_CROSS_BUILD_GENERIC_PATH}")
        set(CMAKE_C_COMPILER "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-gcc.exe")
        set(CMAKE_CXX_COMPILER "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-g++.exe")
        set(CMAKE_ASM_COMPILER "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-gcc.exe")
        set(CMAKE_AR "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-ar.exe")
        set(CMAKE_OBJCOPY "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-objcopy.exe")
        set(CMAKE_OBJDUMP "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-objdump.exe")
        set(SIZE "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-size.exe")
    else()
        message(FATAL_ERROR "NOT DEFINED ARM_CROSS_BUILD_GENERIC_PATH VARIABLES")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/arm-none-eabi-g++")
    set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-gcc")
    set(CMAKE_AR "/usr/bin/arm-none-eabi-ar")
    set(CMAKE_OBJCOPY "/usr/bin/arm-none-eabi-objcopy")
    set(CMAKE_OBJDUMP "/usr/bin/arm-none-eabi-objdump")
    set(SIZE "/usr/bin/arm-none-eabi-size")
endif ()

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Xlinker -v")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections -fdiagnostics-color=auto")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics -fdiagnostics-color=auto")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()