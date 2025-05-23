add_library(sysclock PRIVATE sysclock.c) 
target_include_directories(sysclock PRIVATE 
                            ${PROJECT_ROOT_DIR}/Project/Include/SysClock
                            ${STM32_SPL_INCLUDES}
                            )