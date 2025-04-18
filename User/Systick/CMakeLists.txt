# 定义 STM32 底层库 (静态库)
add_library(Systick STATIC) # 或者使用 OBJECT 库: add_library(stm32_ll_base OBJECT)
file(GLOB SYSTICK_SOURCES "src/*.c")


set(SYSTICK_BASE_SRC ${SYSTICK_SOURCES})
target_sources(Systick PRIVATE ${SYSTICK_BASE_SRC})
target_link_libraries(Systick PRIVATE stm32_spl_base)

target_include_directories(Systick PUBLIC
        ${PROJECT_SOURCE_DIR}/User/Include
        ${PROJECT_SOURCE_DIR}/User/Include/Systick

)