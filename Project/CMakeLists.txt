set (PROJECT_NAME             ${CMAKE_PROJECT_NAME})

set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})
set(BASICCOMPENTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/BasicCompents)

#STM32_SPL define
file(GLOB_RECURSE STM32_SPL_SOURCES ${BASICCOMPENTS_DIR}/STM32_SPL/STM32F4xx_StdPeriph_Driver/src/*.c)
set(STM32_SPL_INCLUDES ${BASICCOMPENTS_DIR}/STM32_SPL/STM32F4xx_StdPeriph_Driver/inc)


set(PROJECT_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/CMSIS/Include       #  CMSIS
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/STM32_SPL/Port      #  STM32_SPL port
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/STM32_SPL/Device    #  STM32_SPL Device
        ${STM32_SPL_INCLUDES}                                   #  STM32_SPL
)




set(PROJECT_SOURCES 
        # LIST SOURCE FILES HERE
        ${BASICCOMPENTS_DIR}/Startup/startup_stm32f407vgtx.s
        main.c
        ${BASICCOMPENTS_DIR}/Syscall/syscalls.c
        ${BASICCOMPENTS_DIR}/Syscall/sysmem.c
        ${STM32_SPL_SOURCES}
)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
        STM32F40_41xxx
        USE_STDPERIPH_DRIVER
        HSE_VALUE=8000000
)

#################################BUILD PROJECT##########################################
project(${EXECUTABLE_NAME})
enable_language(ASM)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


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
add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

