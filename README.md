# 神经网络系统 (Neural Network System)

## 项目概述

本项目是 [ [2025 年 OOP 的作业]](2025OOP大作业_订正版V2.pdf)，拥有一个完整的面向对象神经网络系统，采用现代C++设计原则实现。系统支持多层前馈神经网络的构建、训练、推理和序列化功能，具有模块化架构和专业的测试框架。

## 项目结构

```
OOP/
├── README.md                    # 项目主文档
├── compiler.bat                 # 编译脚本
├── main.cpp                     # 主程序入口
│
├── model/                       # 神经网络核心模型
│   ├── neural_components/       # 神经网络组件
│   │   ├── Network.hpp/cpp      # 神经网络类
│   │   ├── Layer.hpp/cpp        # 网络层类
│   │   ├── Neuron.hpp/cpp       # 神经元类
│   │   └── Synapse.hpp/cpp      # 突触连接类
│   └── activation_functions/    # 激活函数
│       ├── ActivationFunction.hpp/cpp  # 激活函数基类
│       ├── LinearFunction.hpp/cpp      # 线性函数
│       ├── SigmoidFunction.hpp/cpp     # Sigmoid函数
│       ├── TanhFunction.hpp/cpp        # Tanh函数
│       └── ReLUFunction.hpp/cpp        # ReLU函数
│
├── controller/                  # 控制层
│   ├── NetworkController.hpp   # 网络控制器类声明
│   └── NetworkController.cpp   # 网络控制器类实现
│
├── importer/                    # 导入模块
│   ├── BaseImporter.hpp/cpp     # 导入器基类
│   └── ANNImporter.hpp/cpp      # ANN格式导入器
│
├── exporter/                    # 导出模块
│   ├── BaseExporter.hpp/cpp     # 导出器基类
│   └── ANNExporter.hpp/cpp      # ANN格式导出器
│
├── utils/                       # 工具模块
│   ├── FileUtils.hpp            # 文件工具类声明
│   └── FileUtils.cpp            # 文件工具类实现
│
├── interface/                   # 用户界面
│   ├── ConsoleInterface.hpp     # 控制台界面类声明
│   └── ConsoleInterface.cpp     # 控制台界面类实现
│
└── tests/                       # 测试框架
    ├── README.md                # 测试文档
    ├── NeuralNetworkTester.hpp  # 测试器类声明
    ├── NeuralNetworkTester.cpp  # 测试器类实现
    ├── complex.ANN              # 测试网络文件
    └── test.exe                 # 测试程序
```

## Environment

### 我的环境

- VSCODE
  ```
  Version: 1.101.2 (system setup)
  Commit: 2901c5ac6db8a986a5666c3af51ff804d05af0d4
  Date: 2025-06-24T20:27:15.391Z
  Electron: 35.5.1
  ElectronBuildId: 11727614
  Chromium: 134.0.6998.205
  Node.js: 22.15.1
  V8: 13.4.114.21-electron.0
  OS: Windows_NT x64 10.0.26100
  ```
- C++ 14
   ```
   c++.exe (x86_64-posix-seh-rev0, Built by MinGW-Builds project) 13.2.0
   ```
- 系统环境
   ```
   Windows 11 Pro, 24H2, 26100.4770, 64-bit operating system
   ```
### 编译项目

```bash
# 使用提供的编译脚本
.\compiler.bat

# 或手动编译
g++ -std=c++14 -Wall -Wextra -O2 -o neural_network main.cpp ^
    model/activation_functions/*.cpp ^
    model/neural_components/*.cpp ^
    controller/*.cpp ^
    interface/*.cpp ^
    utils/*.cpp ^
    importer/*.cpp ^
    exporter/*.cpp
```

### 运行程序

```bash
# 运行主程序
./neural_network

# 运行测试套件
cd tests
./test.exe
```

## 使用指南

### 基本操作

1. **启动程序**
   ```bash
   ./neural_network
   ```

2. **创建神经网络**
   - 选择菜单选项创建新网络
   - 指定层数和每层神经元数量
   - 选择激活函数类型

3. **导入已有网络**
   - 从ANN文件加载预训练网络
   - 系统自动验证网络结构

4. **网络推理**
   - 输入数据向量
   - 获取网络输出结果

5. **保存网络**
   - 将当前网络保存为ANN文件
   - 支持后续加载使用

### ANN文件格式

ANN文件采用文本格式存储网络结构：
```
# 网络层数
3
# 第1层信息
3 Linear
# 神经元1: 偏置 连接数 [目标神经元:权重]...
0 3 1:0.5 1:0.3 1:-0.2
0 3 1:0.1 1:0.7 1:0.4
0 3 1:-0.3 1:0.2 1:0.8
# 第2层信息
3 Sigmoid
...
```
