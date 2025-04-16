# 定义 STM32 底层库 (静态库)
add_library(cmbacktrace STATIC) # 或者使用 OBJECT 库: add_library(stm32_ll_base OBJECT)
file(GLOB CM_SOURCES "src/*.c")


set(CM_SRC
        ${CM_SOURCES}
)
target_sources(cmbacktrace PRIVATE ${CM_SRC})


# --- 包含目录 ---
# 使用 PUBLIC，这样链接到此库的目标会自动获得这些包含路径
target_include_directories(cmbacktrace PUBLIC
        inc # 包含 stm32f4xx.h, main.h (如果 CubeMX 生成), stm32f4xx_ll_conf.h 等

)
