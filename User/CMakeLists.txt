# 获取顶层项目名称 (在顶层 CMakeLists.txt 中定义的)
set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})

add_subdirectory(Systick)
# 添加可执行文件目标
add_executable(${EXECUTABLE_NAME}
        main.c
        stm32f4xx_it.c
        SysCall/src/sysmem.c
        SysCall/src/syscalls.c  #系统调用不生成静态库
)
# --- 应用层包含目录 ---
target_include_directories(${EXECUTABLE_NAME} PRIVATE
        Include/CMSIS
        Include/Core # App/Include 目录，存放你的应用层头文件
        Include/Systick
)
# --- 链接底层库 ---

target_link_libraries(${EXECUTABLE_NAME} PRIVATE stm32_spl_base Segger cmbacktrace Systick)


# --- 应用层编译定义 ---
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE

)

# --- 链接器设置 ---
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32_base/STM32F407VGTX_FLASH.ld)

# 检查 LINKER_SCRIPT 是否有效
if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()

# 设置链接器选项 (非常重要!)
# 这些选项通常在工具链文件中设置，但也可以在这里指定
target_link_options(${EXECUTABLE_NAME} PRIVATE
        -T${LINKER_SCRIPT}
)

# 使 CMake 知道 Linker Script 是一个依赖项
set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

# 将 Map 文件添加到清理列表
set_target_properties(${EXECUTABLE_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${EXECUTABLE_NAME}.map)


# --- Post-Build Steps (生成 .bin/.hex, 显示大小) ---
# 确保你的工具链文件定义了 CMAKE_OBJCOPY 和 CMAKE_SIZE
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE_NAME}> ${EXECUTABLE_NAME}.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE_NAME}> ${EXECUTABLE_NAME}.hex
#        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE_NAME}>
        COMMENT "Generating binary, hex files and printing size"
        VERBATIM # 确保命令按原样传递
)