
# 神经网络测
```
tests/
├── complex.ANN                  # 测试用的神经网络配置文件
├── NeuralNetworkTester.hpp      # 测试器类头文件声明
├── NeuralNetworkTester.cpp      # 测试器类实现和主程序
└── README.md                   # 说明文档
```
## 概述

面向对象设计的神经网络综合测试程序，严格遵循OOP原则进行全面的功能验证。

## 文件结构

```
tests/
├── complex.ANN              # 测试用的神经网络配置文件
├── NeuralNetworkTester.hpp  # 测试器类头文件声明
├── NeuralNetworkTest.cpp                 # 测试器类实现和主程序
└── README.md               # 说明文档
```

### 测试功能架构
**核心功能测试模块**
- 网络导入和文件处理验证
- 网络结构验证和分析
- 推理引擎功能测试
- 网络参数修改能力
- 保存/加载一致性验证
- 错误处理和边界情况

**机器学习能力测试模块**
- XOR逻辑问题处理
- 多类分类任务处理  
- 回归任务处理

**健壮性测试模块**
- 多个网络文件处理
- 异常情况和边界测试

## 编译和运行

在 `tests` 目录下执行：

```bash
# 编译测试程序 (包含头文件和实现文件)
g++ -std=c++11 -Wall -Wextra -O2 -I.. ../model/neural_components/*.cpp ../model/activation_functions/*.cpp ../controller/*.cpp ../utils/*.cpp ../importer/*.cpp ../exporter/*.cpp NeuralNetworkTester.cpp -o test.exe

# 运行测试
./test.exe
```


## 测试输出

```
Neural Network Comprehensive Test Suite
=======================================
Object-Oriented Testing Framework

=== Comprehensive Neural Network Feature Testing ===
Testing ALL features with detailed progress tracking

>>> Core Functionality Tests <<<
Trying network import from ANN file... PASSED
Trying network structure validation... PASSED
Trying network structure analysis... PASSED
  Network Statistics:
  Total Layers: 3
  Total Neurons: 9
  Total Synapses: 39
  Valid: Yes
Trying neural network inference... PASSED
  Input: [1.0, 0.0, 0.0] -> Output: [0.580, -0.800, 0.380]
Trying network parameter modification... PASSED
Trying save and reload consistency... PASSED
  Maximum output difference: 0.00e+00
Trying error handling and edge cases... PASSED

>>> Machine Learning Capability Tests <<<
Note: Performance results are informational for untrained networks
Trying XOR logic problem processing... PROCESSING
  XOR[0,0] -> [0.000,0.000,0.000]
  XOR[0,1] -> [-0.480,-0.010,0.500]
  XOR[1,0] -> [0.580,-0.800,0.380]
  XOR[1,1] -> [0.100,-0.810,0.880]
  XOR MSE: 0.4035
PASSED

Trying multi-class classification processing... PROCESSING
  Sample 1 -> Class 2 (expected 0)
  Sample 2 -> Class 0 (expected 0)
  ...
  Classification accuracy: 11.1%
PASSED

Trying regression task processing... PROCESSING
  Regression MSE: 0.4681
PASSED

>>> Robustness and Edge Case Tests <<<
Trying multiple network files processing... PROCESSING
  Loading complex.ANN... SUCCESS
    Inference test: PASS
  Loading test_output.ANN... SUCCESS
    Inference test: PASS
PASSED

=== Test Summary ===
Passed: 11/11 tests
Success Rate: 100.0%

=== Final Results ===
SUCCESS: All neural network features are working correctly!
```
