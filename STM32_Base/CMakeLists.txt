# 定义 STM32 底层库 (静态库)
add_library(stm32_spl_base STATIC) # 或者使用 OBJECT 库: add_library(stm32_ll_base OBJECT)
file(GLOB SPL_DRIVER_SOURCES "Drivers/STM32F4xx_StdPeriph_Driver/src/*.c")

# --- 源文件 ---
set(STM32_BASE_SRC
        Src/system_stm32f4xx.c
        ${SPL_DRIVER_SOURCES}
        # --- 启动文件 ---
        startup_stm32f40xx.s
)
target_sources(stm32_spl_base PRIVATE ${STM32_BASE_SRC})


# --- 包含目录 ---
# 使用 PUBLIC，这样链接到此库的目标会自动获得这些包含路径
target_include_directories(stm32_spl_base PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc # 包含 stm32f4xx.h, main.h (如果 CubeMX 生成), stm32f4xx_ll_conf.h 等
        ${PROJECT_SOURCE_DIR}/User/Include/CMSIS/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_StdPeriph_Driver/inc
)

# --- 编译定义 ---
# 使用 PUBLIC，这样链接到此库的目标会自动获得这些定义
target_compile_definitions(stm32_spl_base PUBLIC
        USE_STDPERIPH_DRIVER   # 或者根据需要使用 USE_LL_xxx_DRIVER 宏
        STM32F40_41xxx         # 你的 MCU 型号
        HSE_VALUE=8000000
        __FPU_PRESENT=1
        __CM4__
)

set(STM32_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld CACHE FILEPATH "Linker Script")