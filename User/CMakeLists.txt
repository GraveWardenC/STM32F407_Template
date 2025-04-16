# 获取顶层项目名称 (在顶层 CMakeLists.txt 中定义的)
set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})

# 添加可执行文件目标
add_executable(${EXECUTABLE_NAME}
        Src/main.c
        Src/stm32f4xx_it.c       # 中断处理函数
        Src/syscalls.c
        Src/sysmem.c
)

# --- 链接底层库 ---
# 使用 PRIVATE，因为只有这个可执行文件直接链接 stm32_ll_base
target_link_libraries(${EXECUTABLE_NAME} PRIVATE stm32_spl_base Segger cmbacktrace)

# --- 应用层包含目录 ---
target_include_directories(${EXECUTABLE_NAME} PRIVATE
        Inc # App/Inc 目录，存放你的应用层头文件
)

# --- 应用层编译定义 ---
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
        # 添加你应用层需要的宏定义
)

# --- 链接器设置 ---
# 从 stm32_base 获取 Linker Script 路径 (如果上面设置了全局属性或缓存变量)
# 或者直接使用相对路径:
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32_base/STM32F407VGTX_FLASH.ld)
#get_property(LINKER_SCRIPT GLOBAL PROPERTY STM32_LINKER_SCRIPT) # 如果使用 set_property(GLOBAL...)
# 或者直接引用 Cache 变量: set(LINKER_SCRIPT ${STM32_LINKER_SCRIPT})

# 检查 LINKER_SCRIPT 是否有效
if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()

# 设置链接器选项 (非常重要!)
# 这些选项通常在工具链文件中设置，但也可以在这里指定
target_link_options(${EXECUTABLE_NAME} PRIVATE
        -T${LINKER_SCRIPT}
        # --- 以下选项通常在工具链文件中设置 ---
        # -mcpu=cortex-m4
        # -mthumb
        # --specs=nosys.specs
        # --specs=nano.specs # 如果使用 newlib-nano
        # -Wl,-Map=${CMAKE_PROJECT_NAME}.map # 生成 Map 文件
        # -Wl,--gc-sections # 移除未使用的代码段
        # ---------------------------------------
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