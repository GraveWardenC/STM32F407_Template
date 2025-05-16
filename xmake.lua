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
    
    after_build(function (target)
        -- local elf = target:targetfile()
        -- print(">>> ELF 段信息:")
        -- os.exec("arm-none-eabi-size -A %s", elf)
        -- print("\n>>> 汇总信息:")
        -- os.exec("arm-none-eabi-size %s", elf)
        local elf = target:targetfile()
        local flash_limit = 1024 * 1024    -- 1MB Flash
        local ram_limit   = 128 * 1024     -- 128KB RAM
        local ccm_limit   = 64 * 1024      -- 64KB CCMRAM

        -- 获取段大小
        local result = os.iorunv("arm-none-eabi-size", { elf })
        local text, data, bss = result:match("(%d+)%s+(%d+)%s+(%d+)")
        if not text or not data or not bss then
            print("arm-none-eabi-size 输出解析失败:\n" .. result)
            return
        end

        text, data, bss = tonumber(text), tonumber(data), tonumber(bss)

        local rom_used = text + data
        local ram_used = data + bss
        local ccm_used = 0  -- 可扩展，如果使用了 .ccmram 段

    -- 输出格式美化
        local function format_size(bytes)
            if bytes >= 1024 * 1024 then
                return string.format("%.0f MB", bytes / (1024 * 1024))
            elseif bytes >= 1024 then
                return string.format("%.0f KB", bytes / 1024)
            else
                return string.format("%d B", bytes)
            end
        end

        print("\n>>> 生成 BIN 并显示大小:")
        local bin = path.join(target:targetdir(), "APP.bin")
        os.exec("arm-none-eabi-objcopy -O binary %s %s", elf, bin)
        os.exec("ls -lh %s", bin)
        
        print("\nMemory region         Used Size  Region Size  age Used()")
        print(string.format("%16s: %10s %10s %10.2f%%", "CCMRAM", string.format("%d B", ccm_used), format_size(ccm_limit), 100 * ccm_used / ccm_limit))
        print(string.format("%16s: %10s %10s %10.2f%%", "RAM",string.format("%d B", ram_used) , format_size(ram_limit), 100 * ram_used / ram_limit))
        print(string.format("%16s: %10s %10s %10.2f%%", "ROM",string.format("%d B", rom_used), format_size(flash_limit), 100 * rom_used / flash_limit))
    end)

