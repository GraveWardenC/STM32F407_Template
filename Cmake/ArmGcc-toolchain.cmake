# build for arm 
set (CMAKE_SYSTEM_PROCESSOR "arm" CACHE STRING "")
set (CMAKE_SYSTEM_NAME "Generic" CACHE STRING "")

# Skip link step during toolchain validation.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

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
        set(CMAKE_SIZE "$ENV{ARM_CROSS_BUILD_GENERIC_PATH}/arm-none-eabi-size.exe")
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
    set(CMAKE_SIZE "/usr/bin/arm-none-eabi-size")
endif ()

# toolchain flags
set(MCPU                        "-mcpu=cortex-m4")
set(MFPU                        "-mfpu=fpv4-sp-d16")
set(MFLOAT_ABI                  "-mfloat-abi=hard")
set(RUNTIME_LIBRARY             "--specs=nano.specs")
set(RUNTIME_LIBRARY_SYSCALLS    "--specs=nosys.specs")
set(LINKER_SCRIPT              "../STM32F407VGTX_FLASH.ld")
set(MAP_SET                     "-Wl,-Map=${PROJECT_ROOT_DIR}/Package/Output/Project.map")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")
set (CMAKE_EXECUTABLE_SUFFIX        ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX    ".a")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


set (CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror=uninitialized -O0 -g3 -ggdb")
set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} ${MAP_SET} -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g0")


# if(CMAKE_BUILD_TYPE STREQUAL "Release")
#     add_definitions(-DRELEASE)
# elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_definitions(-DDEBUG)
# endif()