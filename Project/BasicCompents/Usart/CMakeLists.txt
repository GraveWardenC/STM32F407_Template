add_library(usart
    usart.c
)

target_include_directories(usart PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # 包含 usart.h 所在路径
    ${CMSIS_INCLUDES}
    ${STM32_PORT_INCLUDES}
    ${STM32_DEVICE_INCLUDES}
    ${STM32_SPL_INCLUDES}
)
target_compile_definitions(usart PRIVATE ${PROJECT_DEFINES})