# 定义 STM32 底层库 (静态库)
add_library(Segger STATIC) # 或者使用 OBJECT 库: add_library(stm32_ll_base OBJECT)
file(GLOB SEGGER_SOURCES "src/*.c")


set(SEGGER_BASE_SRC ${SEGGER_SOURCES})
target_sources(Segger PRIVATE ${SEGGER_BASE_SRC})

target_include_directories(Segger PUBLIC
        inc
)
