# STM32F103C8T6 编译与烧录指南

本文档为 AI 助手（Claude、其他 LLM）提供在本项目目录下编译和烧录 STM32 的完整操作指南。

## 环境要求

本项目依赖 STM32CubeIDE 提供的工具链，路径如下：

| 工具 | 路径 |
|------|------|
| ARM GCC 编译器 | `C:\Users\ChengCheng\AppData\Local\stm32cube\bundles\gnu-tools-for-stm32\14.3.1+st.2\bin` |
| CMake | `C:\Users\ChengCheng\AppData\Local\stm32cube\bundles\cmake\4.0.1+st.3\bin` |
| Ninja | `C:\Users\ChengCheng\AppData\Local\stm32cube\bundles\ninja\1.13.1+st.1\bin` |
| STM32 Programmer CLI | `C:\Users\ChengCheng\AppData\Local\stm32cube\bundles\programmer\2.22.0+st.1\bin\STM32_Programmer_CLI.exe` |

## 完整构建流程（推荐）

### 1. 首次构建（或CMake配置变更时）
```bash
export PATH="/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/gnu-tools-for-stm32/14.3.1+st.2/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH" && cd "e:/Project/Programming/Project/STM32/FishPool.buy" && cmake --preset debug
```

### 2. 编译项目
```bash
export PATH="/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/gnu-tools-for-stm32/14.3.1+st.2/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH" && cd "e:/Project/Programming/Project/STM32/FishPool.buy" && cmake --build --preset debug
```

### 3. 烧录程序
```bash
"/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/programmer/2.22.0+st.1/bin/STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "e:/Project/Programming/Project/STM32/FishPool.buy/build/debug/FishPool_buy.elf" -rst
```

## 快速命令（已配置好CMake时）

### 编译命令
```bash
export PATH="/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/gnu-tools-for-stm32/14.3.1+st.2/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH" && cd "e:/Project/Programming/Project/STM32/FishPool.buy/build/debug" && ninja
```

### 烧录命令
```bash
"/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/programmer/2.22.0+st.1/bin/STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "e:/Project/Programming/Project/STM32/FishPool.buy/build/debug/FishPool_buy.elf" -rst
```

### 编译+烧录合并命令
```bash
export PATH="/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/gnu-tools-for-stm32/14.3.1+st.2/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH" && cd "e:/Project/Programming/Project/STM32/FishPool.buy/build/debug" && ninja && "/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/programmer/2.22.0+st.1/bin/STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "e:/Project/Programming/Project/STM32/FishPool.buy/build/debug/FishPool_buy.elf" -rst
```

## 项目结构

```
FishPool.buy/
├── Src/              # 源代码 (.c)
├── Inc/              # 头文件 (.h)
├── Libraries/        # STM32 标准外设库 + CMSIS + 用户驱动
│   ├── CMSIS/        # Cortex Microcontroller Software Interface Standard
│   │   └── startup/arm/startup_stm32f10x_md.S  # 启动文件（重要！）
│   ├── STM32F10x_StdPeriph_Driver/  # STM32标准外设库
│   └── User/         # 用户自定义驱动模块
├── Startup/          # 启动文件（备用）
├── build/debug/      # 编译输出目录
│   ├── FishPool_buy.elf  # ELF 文件（烧录用）
│   ├── FishPool_buy.hex  # HEX 文件
│   ├── FishPool_buy.bin  # BIN 文件
│   └── build.ninja   # Ninja 构建文件
├── CMakeLists.txt    # CMake 配置
├── CMakePresets.json # CMake 预设（debug/release等）
├── stm32f103c8tx_FLASH.ld  # 链接器脚本
└── cmake/gcc-arm-none-eabi.cmake  # ARM 工具链配置
```

## 编译输出含义

编译成功后会显示内存使用情况：

```
Memory region         Used Size  Region Size  %age Used
             RAM:       10944 B        20 KB     53.44%
           FLASH:       63156 B        64 KB     96.37%
```

STM32F103C8T6 资源：
- RAM: 20 KB
- FLASH: 64 KB

## 常见编译错误处理

1. **找不到 `arm-none-eabi-gcc`** → 使用上述 PATH 设置
2. **找不到 `ninja`** → 使用上述 PATH 设置
3. **语法错误** → 检查 `.c` 文件中的括号、分号、参数完整性
4. **链接错误：`undefined reference to '__main'`** → 启动文件需要修改：
   - 编辑文件：`Libraries/CMSIS/startup/arm/startup_stm32f10x_md.S`
   - 将第140行的 `ldr r0, =__main` 改为 `ldr r0, =main`
   - 原因：GCC ARM工具链期望直接跳转到用户`main`函数，而不是标准库的`__main`
5. **链接错误：其他未定义引用** → 检查CMakeLists.txt中的链接器选项，确保包含`-lgcc`库和`--specs=nosys.specs`

## 关键修复说明

本项目已修复以下关键问题：

### 1. 启动文件修复
启动文件 `Libraries/CMSIS/startup/arm/startup_stm32f10x_md.S` 第140行已从：
```assembly
ldr r0, =__main
```
修改为：
```assembly
ldr r0, =main
```
这是因为ARM GCC工具链在嵌入式环境中期望直接跳转到用户定义的`main`函数，而不是C标准库的初始化例程`__main`。

### 2. 链接器配置
在 `CMakeLists.txt` 中确保链接器选项包含：
- `-lgcc`：GCC运行时库
- `--specs=nosys.specs`：提供系统调用存根实现

## 烧录参数说明

| 参数 | 说明 |
|------|------|
| `-c port=SWD` | 使用 SWD 接口连接 |
| `mode=UR` | Under Reset 模式（适用于某些锁定芯片） |
| `-w xxx.elf` | 写入 ELF 文件 |
| `-rst` | 烧录完成后复位 MCU |

## 用户快捷键（仅供参考）

VSCode 中配置了 `Ctrl+Alt+1` 快捷键，触发 "Flash Program" 任务：
- 先执行 CMake 编译
- 再执行 STM32_Programmer_CLI 烧录

AI 无法直接触发 IDE 快捷键，需使用上述命令行方式。