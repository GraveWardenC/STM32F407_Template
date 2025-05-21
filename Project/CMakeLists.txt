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

project(${EXECUTABLE_NAME})
enable_language(ASM)





add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})



# # --- 应用层编译定义 ---
# target_compile_definitions(${EXECUTABLE_NAME} PRIVATE

# )

# # --- 链接器设置 ---
# set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32_base/STM32F407VGTX_FLASH.ld)

# # 检查 LINKER_SCRIPT 是否有效
# if(NOT EXISTS ${LINKER_SCRIPT})
#     message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
# endif()

# # 设置链接器选项 (非常重要!)
# # 这些选项通常在工具链文件中设置，但也可以在这里指定
# target_link_options(${EXECUTABLE_NAME} PRIVATE
#         -T${LINKER_SCRIPT}
# )

# # 使 CMake 知道 Linker Script 是一个依赖项
# set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

# # 将 Map 文件添加到清理列表
# set_target_properties(${EXECUTABLE_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${EXECUTABLE_NAME}.map)


# # --- Post-Build Steps (生成 .bin/.hex, 显示大小) ---
# # 确保你的工具链文件定义了 CMAKE_OBJCOPY 和 CMAKE_SIZE
# add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
#         COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE_NAME}> ${EXECUTABLE_NAME}.bin
#         COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE_NAME}> ${EXECUTABLE_NAME}.hex
# #        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE_NAME}>
#         COMMENT "Generating binary, hex files and printing size"
#         VERBATIM # 确保命令按原样传递
# )