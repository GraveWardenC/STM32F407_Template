set_plat("arm")
set_toolchains("armgcc")  -- 指定交叉编译工具链

-- 项目配置
set_project("stm32f407_project")
set_version("1.0")



-- 获取 ARM 工具链路径
local arm_gcc_prefix = os.getenv("ARM_CROSS_BUILD_GENERIC_PATH")
if not arm_gcc_prefix then
    print("请设置 ARM_COMPILER 环境变量为 arm-none-eabi 工具链的路径")
end

-- 工具链配置
toolchain("armgcc")
    set_kind("standalone")
    set_toolset("cc", path.join(arm_gcc_prefix, "arm-none-eabi-gcc.exe"))
    set_toolset("ld", path.join(arm_gcc_prefix, "arm-none-eabi-gcc.exe"))
    set_toolset("ar", path.join(arm_gcc_prefix, "arm-none-eabi-ar.exe"))
    set_toolset("as", path.join(arm_gcc_prefix, "arm-none-eabi-gcc.exe"))
    set_toolset("objcopy", path.join(arm_gcc_prefix, "arm-none-eabi-objcopy"))
    set_toolset("objdump", path.join(arm_gcc_prefix, "arm-none-eabi-objdump"))
    set_toolset("size",    path.join(arm_gcc_prefix, "arm-none-eabi-size"))
toolchain_end()

-- 公共编译标志
local common_flags = {
    "-mcpu=cortex-m4",
    "-mthumb",
    "-mfpu=fpv4-sp-d16",
    "-DSTM32F40_41xxx",     -- 定义芯片系列
    "-DUSE_STDPERIPH_DRIVER",
    "-DHSE_VALUE=8000000",  -- 外部晶振 8MHz
    "-ffunction-sections",
    "-fdata-sections",
    "-Wall"
}

-- 编译 SPL 为静态库
target("stm32f4spl")
    set_kind("static")
    set_toolchains("armgcc")
    set_targetdir("bin/liboutput")
    add_cflags(table.unpack(common_flags))
    add_includedirs("Drivers/CMSIS/Include",
                    "Drivers/CMSIS/Device/ST/STM32F4xx/Include",
                    "Drivers/STM32F4xx_StdPeriph_Driver/inc",
                    "Core/Inc")   -- 添加头文件目录
    add_files("Drivers/STM32F4xx_StdPeriph_Driver/src/*.c")

target("APP")
    set_kind("binary")
    set_extension(".elf")
    set_toolchains("armgcc")
    set_targetdir("bin/liboutput")
    add_cflags(table.unpack(common_flags))
    add_includedirs(
        "Drivers/CMSIS/Include",
        "Drivers/CMSIS/Device/ST/STM32F4xx/Include",
        "Drivers/STM32F4xx_StdPeriph_Driver/inc",
        "Core/inc"
    )
    add_files("Core/src/*.c")
    add_files("Board/*.s")
    -- add_links("bin/liboutput/libstm32f4spl.a")
    add_deps("stm32f4spl")
    add_ldflags("-T Board/STM32F407VGTX_FLASH.ld")

