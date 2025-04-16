# 定义 STM32 底层库 (静态库)
add_library(stm32_spl_base STATIC) # 或者使用 OBJECT 库: add_library(stm32_ll_base OBJECT)
file(GLOB SPL_DRIVER_SOURCES "Drivers/STM32F4xx_StdPeriph_Driver/src/*.c")

# --- 源文件 ---
# 注意：这里需要列出所有 *LL* 库文件，而不是 HAL 库文件
# CubeMX 生成 HAL 时，你需要手动修改为 LL
set(STM32_BASE_SRC
        Src/system_stm32f4xx.c
        # --- LL 驱动 ---
        # !!*** 请根据你 CubeMX 配置中实际使用的 LL 模块添加对应的 .c 文件 ***!!
        ${SPL_DRIVER_SOURCES}
        # ... 添加其他你使用的 LL 驱动源文件 (例如: ll_usart.c, ll_spi.c, ll_i2c.c 等)

        # --- 启动文件 ---
        startup_stm32f40xx.s

        # --- 其他基础文件 (如果需要) ---
        # 根据你的项目决定是否需要 sysmem.c / syscalls.c，以及它们属于底层还是应用层
        # Src/sysmem.c
        # Src/syscalls.c
)
target_sources(stm32_spl_base PRIVATE ${STM32_BASE_SRC})


# --- 包含目录 ---
# 使用 PUBLIC，这样链接到此库的目标会自动获得这些包含路径
target_include_directories(stm32_spl_base PUBLIC
        Inc # 包含 stm32f4xx.h, main.h (如果 CubeMX 生成), stm32f4xx_ll_conf.h 等
        Drivers/STM32F4xx_StdPeriph_Driver/Inc
        CMSIS/Include
)

# --- 编译定义 ---
# 使用 PUBLIC，这样链接到此库的目标会自动获得这些定义
target_compile_definitions(stm32_spl_base PUBLIC
        # !!*** 确保这里使用 LL 相关的宏 ***!!
        USE_STDPERIPH_DRIVER   # 或者根据需要使用 USE_LL_xxx_DRIVER 宏
        STM32F40_41xxx         # 你的 MCU 型号
        # 根据你的时钟配置添加 HSE_VALUE, HSI_VALUE 等 (如果 system_stm32f4xx.c 需要)
        HSE_VALUE=8000000
        # HSI_VALUE=16000000
        $<$<CONFIG:Debug>:DEBUG> # 只在 Debug 模式下添加 DEBUG 宏
)

# --- C 标准验证 (可选) ---
# 这个检查可以放在顶层 CMakeLists.txt 中
# if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
#     message(FATAL_ERROR "Generated code requires C11 or higher")
# endif()

# --- Linker Script ---
# 将 Linker Script 的路径设置为一个变量，供 App 使用
# 路径相对于顶层 CMakeLists.txt
set(STM32_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld CACHE FILEPATH "Linker Script")
# 可以将此变量设置为 CACHE 变量或使用 set_property(GLOBAL ...) 使其全局可见
# 或者在 App/CMakeLists.txt 中直接引用相对路径