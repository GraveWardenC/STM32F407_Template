set (PROJECT_NAME             ${CMAKE_PROJECT_NAME})

set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})
set(BASICCOMPENTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/BasicCompents)

set(MAIN_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Include)
#Basic compents STM32_HAL define Do not use module
set(CMSIS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Include/CMSIS/Include)
set(STM32_DEVICE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Include/STM32_HAL/Device)    #  STM32_HAL Device
set(STM32_PORT_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Include/STM32_HAL/Port)    #  STM32_HAL port

file(GLOB_RECURSE STM32_HAL_SOURCE 
        ${BASICCOMPENTS_DIR}/STM32_HAL/STM32F4xx_HAL_Driver/src/*.c 
        ${BASICCOMPENTS_DIR}/STM32_HAL/system_stm32f4xx.c
)
set(STM32_HAL_INCLUDES ${BASICCOMPENTS_DIR}/STM32_HAL/STM32F4xx_HAL_Driver/inc)

#Basic compents FreeRtos define Do not use module
# file(GLOB_RECURSE RTOS_SOURCES ${BASICCOMPENTS_DIR}/FreeRtos_LTS_20240601/*.c)
# set(RTOS_INCLUDES 
#         ${CMAKE_CURRENT_SOURCE_DIR}/Include/FreeRtos_Cfg
#         ${BASICCOMPENTS_DIR}/FreeRtos_LTS_20240601/portable
#         ${BASICCOMPENTS_DIR}/FreeRtos_LTS_20240601/include
#         )

set(PROJECT_INCLUDES
        ${CMSIS_INCLUDES}
        ${STM32_PORT_INCLUDES}
        ${STM32_HAL_INCLUDES}                                   #  STM32_HAL
        ${STM32_DEVICE_INCLUDES}
        ${MAIN_INCLUDES}

)


set(PROJECT_SOURCES 
        # LIST SOURCE FILES HERE
        ${BASICCOMPENTS_DIR}/Startup/startup_stm32f407vgtx.s
        main.c
        stm32f4xx_it.c
        ${BASICCOMPENTS_DIR}/Syscall/syscalls.c
        ${BASICCOMPENTS_DIR}/Syscall/sysmem.c
        ${STM32_HAL_SOURCE}
)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
        STM32F407xx
        USE_HAL_DRIVER
        HSE_VALUE=8000000
        CACHE INTERNAL "Project compile definitions"
)

#################################BUILD PROJECT##########################################
project(${EXECUTABLE_NAME})
enable_language(ASM)



add_subdirectory(BasicCompents)

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
        SeggerRTT
        # FatFs
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>

    # hex
    COMMAND ${CMAKE_OBJCOPY} -O ihex 
    $<TARGET_FILE:${PROJECT_NAME}> 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex 

    # bin
    COMMAND ${CMAKE_OBJCOPY} -O binary 
    $<TARGET_FILE:${PROJECT_NAME}> 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin

    # size
    COMMAND ${CMAKE_SIZE} -A 
    $<TARGET_FILE:${PROJECT_NAME}> > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_size.txt
)


