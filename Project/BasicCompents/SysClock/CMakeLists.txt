add_library(sysclock
    sysclock.c
)

target_include_directories(sysclock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # 包含 sysclock.h 所在路径
    ${PROJECT_ROOT_DIR}/Project/Include
    ${CMSIS_INCLUDES}
    ${STM32_PORT_INCLUDES}
    ${STM32_DEVICE_INCLUDES}
    ${STM32_SPL_INCLUDES}
)
target_compile_definitions(sysclock PRIVATE ${PROJECT_DEFINES})
