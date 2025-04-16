# F4_SPL_NONE

这是一个基于 **STM32F4** 系列 MCU 的模板工程，使用 **Standard Peripheral Library (SPL)** 和 **CMake** 构建，适配交叉编译工具链 `arm-none-eabi-gcc`，支持调试、优化、模块化开发。


## 🧰 编译环境

- **Toolchain:** `arm-none-eabi-gcc`
- **构建系统:** `CMake >= 3.20`
- **调试器:** `OpenOCD` / `JLinkGdb`
- **系统:** `Windows` / `Linux`

## 📦 第三方组件

- SeggerRTT: 🔗<a href="https://kb.segger.com/RTT">Segger RTT</a>
- CmbackTrace

## 🚧 TODO
- [x] 📚 增加文档说明 
- [x] 🔧 使用Segger RTT作为Debug log
- [ ] ⚙️ 添加 SysTick 定时器模板作为系统硬件级延时时基

