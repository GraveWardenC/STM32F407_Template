# F4_SPL_NONE

这是一个基于 **STM32F4** 系列 MCU 的模板工程，使用 **Standard Peripheral Library (SPL)** 和 **CMake** 构建，适配交叉编译工具链 `arm-none-eabi-gcc`，支持调试、优化、模块化开发。


## 🧰 编译环境

- **Toolchain:** `arm-none-eabi-gcc`
- **构建系统:** `CMake >= 3.20`
- **调试器:** `JLinkGdb`
- **系统:** `Windows` / `Linux`

## 🔨 配置

### `arm-none-eabi-gcc` 配置

1. 下载编译链并配置环境变量

    存放地址：南京文件服务器：\\192.168.2.88\个人文件\chenkai\Windows\ArmToolchain

    下载改编译链安装至一个固定的文件夹，并将bin目录添加至环境变量path中
    ![图片描述](Doc\img\2.png)
    ![图片描述](Doc\img\1.png)
    这一步是为了相关工具能够直接在系统shell中调用执行。比如size nm等工具

    新建一个系统变量，变量名为 `ARM_CROSS_BUILD_GENERIC_PATH`，值为编译链安装路径，如：`C:\Program Files\ArmToolchain\gcc-arm-none-eabi-10.3-2021.10\bin`，这一步你需要将`\`路径符修改为`/`，防止在windows下cmake路径符导致的问题，此问题只会在vscode下存在。
    ![图片描述](Doc\img\3.png)
    需要额外配置这一步的原因是Cmake/ArmGcc-toolchain.cmake脚本使用环境变量获取编译链位置进行交叉编译，如果环境变量未配置，则编译链位置将无法识别，cmake配置失败。
    如果需要检测，重启powershell，输入以下命令

    ```shell
    #  查看环境变量是否设置成功
    echo $env:ARM_CROSS_BUILD_GENERIC_PATH
    #  查看arm-none-eabi-gnu相关编译链和工具是否在环境中
    arm-none-eabi-gcc --version
    ```

    期望输出如下
    ![图片描述](Doc\img\gcc.png)

2. 配置Cmake和Ninja
    
    存放地址：

    Ninja:南京文件服务器：\\192.168.2.88\个人文件\chenkai\Windows\Ninja

    Cmake:南京文件服务器：\\192.168.2.88\个人文件\chenkai\Windows\Cmake

    将各自的bin目录添加至环境变量path即可，检测如下

    ```shell
    cmake --version
    ninja --version
    ```
    
    期望输出如下
    ![图片描述](Doc\img\cmake.png)

3. 配置调试器和下载器

    目前有多种可以下载和调试的工具，如SeggerJlink,OpenOcd,Pyocd,STLink等，这些工具的使用和配置方法是类似的，jlink是支持最好，附加工具最多的调试器，因此本示例以Jlink为例。

    首先下载jlink软件包，下载地址：https://www.segger.com/downloads/jlink/
    ![图片描述](Doc\img\4.png)
    下载安装完成后将包含jlink相关软件的路径添加到环境变量path中
    ![图片描述](Doc\img\5.png)

    检测如下：

    ```shell
    jlink --version
    ```
    
    期望输出如下
    ![图片描述](Doc\img\jlink.png)

4. 配置VSCode

    你需要安装以下插件来配置本项目，以提高开发体验。

    C/C++ C/C++ Extensions: 🔗<a href="https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools">C/C++</a>
    ![图片描述](Doc\img\vs1.png)

    CMakeTools: 🔗<a href="https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools">CMake Tools</a>
    ![图片描述](Doc\img\vs2.png)

    Cortex-Debug: 🔗<a href="https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug">Cortex Debug</a>
    ![图片描述](Doc\img\vs3.png)

    Embedded Tools:
    ![图片描述](Doc\img\vs4.png)

    Task Buttons:
    ![图片描述](Doc\img\vs5.png)

## 🚀 快速开始

### Visual Studio Code

1. 克隆项目到本地
2. 使用vscode打开项目，vscode会自动识别cmake文件,如果此时提示选择编译链，需要选择arm-none-eabi-gcc编译链，否则vscode会尝试选择其他的编译链，导致编译失败，也可不选择编译链，手动进行编译命令配置
3. 如果不选择编译链，手动配置编译命令，新建一个终端输入以下shell命令
    ```shell
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    ```
    成功时显示如下
    ![图片描述](Doc\img\vs6.png)

    也可以通过运行自定义tasks来进行操作，tasks.json文件在.vscode目录下，可以自行添加,以下为示例,然后通过运行该任务即可，下同
    ```json
    {
        "label": "cmake-configure",
        "type": "shell",
        "command": "cmake",
        "args": [
            "-B", "build",
            "-G", "Ninja",
            "-DCMAKE_BUILD_TYPE=Debug"
        ],
        "options": {
            "cwd": "${workspaceFolder}"
        },
        "problemMatcher": ["$gcc"],
        "group": "build"
    },
    ```
4. 运行build、flash等任务

    相关任务已经在tasks.json中定义好了，可以直接运行，如果安装了`Task Buttons`插件，则会在vscode底部显示任务按钮，点击即可运行，也可以通过运行自定义tasks来进行操作，tasks.json文件在.vscode目录下，可以自行添加。定义task button按钮的方法见settings.json文件中的`taskButton`配置

    ![图片描述](Doc\img\vs7.png)
    
    你需要右键底部开启task button才会显示自定义任务按键
    ![图片描述](Doc\img\vs8.png)
    
    运行`build`任务的效果如下，需先配置cmake，然后运行`build`任务
    ![图片描述](Doc\img\vs9.png)
    显示生成了一个.elf可执行文件，并且显示了各段长度和可执行文件的位置，在模板工程中该位置位于Package/Output下，并且生成了对应的.bin和.hex文件以及其他描述文件

5. debug支持

    相关debug配置已经在tasks.json中定义。本工程定义了两个debug任务，如图所示
    ![图片描述](Doc\img\vs10.png)
    cortex-debug是使用cortex-debug插件来进行配置的,该插件提供了对cortex内核debug的vscode支持，支持使用jlink,stlink,openocd。

    gdb debug 是使用arm-none-eabi-gdb工具远程连接gdbserver进行调试的，该方法需要自行配置gdb和gdbserver。

    从底层原理来讲,这两种方法的底层原理一致，都是通过gdb和gdbserver进行远程调试，使用cortex-debug插件进行配置相对来说更加容易，但是如果你想了解整个调试过程是如何建立的、或者你想使用其他的GDB server或者配置vscode进行远程调试其他的设备，你可以参考以下gdb debug的设置。

    大部分参数都不需要进行改变，如果你基于此工程创建项目，除了cmake的编写外，在需要进行debug时你可能需要改动以下几点，在调试其他本地c/c++项目时也是类似的
    ![图片描述](Doc\img\vs11.png)

    **注意 在使用gdb bubug 进行测试时，确保在调试前开启了Jlink GDB Server  ，并配置了Jlink GDB Server的端口号为默认值2331**

    进行调试后你会看到如下页面
    ![图片描述](Doc\img\vs12.png)

    ![图片描述](Doc\img\vs13.png)

    安装了peripheral Viewer插件后，会看到外设寄存器视图
    ![图片描述](Doc\img\vs14.png)
    
    安装memory viewer 插件后，会看到内存视图
    ![图片描述](Doc\img\vs15.png)

    可以添加高级断点（条件断点、记录点、触发断点）
    ![图片描述](Doc\img\vs16.png)

### CLion
1. 克隆项目到本地
2.  使用clion打开项目，clion会自动识别根目录下的cmake文件

    此时需要配置编译链以供代码感知和编译使用，例如配置一个MINGW_ST的编译链
    ![图片描述](Doc\img\clion1.png)
    ![图片描述](Doc\img\clion2.png)
    选择配置的编译链，构建器选择默认捆绑的Ninja即可，确认后触发clion的cmake检测，会生成一个名为cmake-build-debug-mingw_st的目录用来存放cmake中间缓存，即默认的build目录，这个目录默认被添加到了.gitignore文件中，因此不会被git管理,也可手动设置目录为build
3. 编译

    此时直接点击编译按键即可进行编译
    ![图片描述](Doc\img\clion3.png)

4. 调试支持

    我们使用vscode中gdb debug原理进行调试，但此时无需建立脚本文件，因为clion2025.1.1之后的版本已经支持了SeggerJlink和ST_LINK的调试，只需要开启相关功能即可。

    首先需要开启远程调试服务器设置
    ![图片描述](Doc\img\clion4.png)
    ![图片描述](Doc\img\clion5.png)
    ![图片描述](Doc\img\clion6.png)
    ![图片描述](Doc\img\clion7.png)
    ![图片描述](Doc\img\clion8.png)
    ![图片描述](Doc\img\clion9.png)
    ![图片描述](Doc\img\clion10.png)


## 📦 第三方组件

- SeggerRTT: 🔗<a href="https://kb.segger.com/RTT">Segger RTT</a>
- CmbackTrace

## 🚧 TODO
- [x] 📚 增加文档说明 
- [x] 🔧 使用Segger RTT作为Debug log
- [ ] ⚙️ 添加 SysTick 定时器模板作为系统硬件级延时时基

