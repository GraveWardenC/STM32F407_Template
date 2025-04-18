set(HARDFAULT_BASE_SRC
        src/SEGGER_HardFaultHandler.c
)

add_library(HardFault STATIC ${HARDFAULT_BASE_SRC})
target_link_libraries(HardFault PRIVATE stm32_spl_base)
