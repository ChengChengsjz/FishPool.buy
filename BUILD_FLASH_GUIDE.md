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

## 一行命令编译

```bash
export PATH="/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/gnu-tools-for-stm32/14.3.1+st.2/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH" && cd "e:/Project/Programming/Project/STM32/FishPool/build/debug" && ninja
```

## 一行命令烧录

```bash
"/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/programmer/2.22.0+st.1/bin/STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "e:/Project/Programming/Project/STM32/FishPool/build/debug/FishPool.elf" -rst
```

## 编译+烧录合并命令

```bash
export PATH="/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/gnu-tools-for-stm32/14.3.1+st.2/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin:/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH" && cd "e:/Project/Programming/Project/STM32/FishPool/build/debug" && ninja && "/c/Users/ChengCheng/AppData/Local/stm32cube/bundles/programmer/2.22.0+st.1/bin/STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "e:/Project/Programming/Project/STM32/FishPool/build/debug/FishPool.elf" -rst
```

## 项目结构

```
FishPool/
├── Src/              # 源代码 (.c)
├── Inc/              # 头文件 (.h)
├── Libraries/        # STM32 标准外设库 + CMSIS + 用户驱动
│   └── User/         # 用户自定义驱动模块
│       ├── usart/    # USART 串口驱动
│       ├── oled/     # OLED 显示驱动
│       ├── delay/    # 延时函数
│       ├── sys/      # 系统配置
│       ├── Timer/    # 定时器驱动
│       └── RotaryEncoder/  # 旋转编码器驱动
├── Startup/          # 启动文件
├── build/debug/      # 编译输出目录
│   ├── FishPool.elf  # ELF 文件（烧录用）
│   ├── FishPool.hex  # HEX 文件
│   ├── FishPool.bin  # BIN 文件
│   └── build.ninja   # Ninja 构建文件
├── CMakeLists.txt    # CMake 配置
├── CMakePresets.json # CMake 预设（debug/release等）
└── cmake/gcc-arm-none-eabi.cmake  # ARM 工具链配置
```

## 编译输出含义

编译成功后会显示内存使用情况：

```
Memory region         Used Size  Region Size  %age Used
             RAM:        3920 B        20 KB     19.14%
           FLASH:        9188 B        64 KB     14.02%
```

STM32F103C8T6 资源：
- RAM: 20 KB
- FLASH: 64 KB

## 常见编译错误处理

1. **找不到 `arm-none-eabi-gcc`** → 使用上述 PATH 设置
2. **找不到 `ninja`** → 使用上述 PATH 设置
3. **语法错误** → 检查 `.c` 文件中的括号、分号、参数完整性

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