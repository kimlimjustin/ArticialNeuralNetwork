//-------------------------------------------------------------
//【文件名】ActivationFunction.cpp
//【功能模块和目的】神经网络激活函数相关实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "ActivationFunction.h"
#include <cmath>
#include <algorithm>

//-------------------------------------------------------------
//【函数名称】activate（SigmoidFunction）
//【函数功能】Sigmoid 激活函数实现
//【参数】x：输入值
//【返回值】double，激活后输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double SigmoidFunction::activate(double x) const {
    return 1.0 / (1.0 + std::exp(-x));
}
//-------------------------------------------------------------
//【函数名称】getName（SigmoidFunction）
//【函数功能】获取函数名称
//【参数】无
//【返回值】std::string，函数名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string SigmoidFunction::getName() const {
    return "Sigmoid";
}
//-------------------------------------------------------------
//【函数名称】clone（SigmoidFunction）
//【函数功能】克隆自身
//【参数】无
//【返回值】std::unique_ptr<ActivationFunction>
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::unique_ptr<ActivationFunction> SigmoidFunction::clone() const {
    return std::unique_ptr<ActivationFunction>(new SigmoidFunction());
}

//-------------------------------------------------------------
//【函数名称】activate（TanhFunction）
//【函数功能】Tanh 激活函数实现
//【参数】x：输入值
//【返回值】double，激活后输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double TanhFunction::activate(double x) const {
    return std::tanh(x);
}
//-------------------------------------------------------------
//【函数名称】getName（TanhFunction）
//【函数功能】获取函数名称
//【参数】无
//【返回值】std::string，函数名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string TanhFunction::getName() const {
    return "Tanh";
}
//-------------------------------------------------------------
//【函数名称】clone（TanhFunction）
//【函数功能】克隆自身
//【参数】无
//【返回值】std::unique_ptr<ActivationFunction>
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::unique_ptr<ActivationFunction> TanhFunction::clone() const {
    return std::unique_ptr<ActivationFunction>(new TanhFunction());
}

//-------------------------------------------------------------
//【函数名称】activate（ReLUFunction）
//【函数功能】ReLU 激活函数实现
//【参数】x：输入值
//【返回值】double，激活后输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double ReLUFunction::activate(double x) const {
    return std::max(0.0, x);
}
//-------------------------------------------------------------
//【函数名称】getName（ReLUFunction）
//【函数功能】获取函数名称
//【参数】无
//【返回值】std::string，函数名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string ReLUFunction::getName() const {
    return "ReLU";
}
//-------------------------------------------------------------
//【函数名称】clone（ReLUFunction）
//【函数功能】克隆自身
//【参数】无
//【返回值】std::unique_ptr<ActivationFunction>
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::unique_ptr<ActivationFunction> ReLUFunction::clone() const {
    return std::unique_ptr<ActivationFunction>(new ReLUFunction());
}

//-------------------------------------------------------------
//【函数名称】activate（LinearFunction）
//【函数功能】Linear 激活函数实现
//【参数】x：输入值
//【返回值】double，激活后输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double LinearFunction::activate(double x) const {
    return x;
}
//-------------------------------------------------------------
//【函数名称】getName（LinearFunction）
//【函数功能】获取函数名称
//【参数】无
//【返回值】std::string，函数名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string LinearFunction::getName() const {
    return "Linear";
}
//-------------------------------------------------------------
//【函数名称】clone（LinearFunction）
//【函数功能】克隆自身
//【参数】无
//【返回值】std::unique_ptr<ActivationFunction>
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::unique_ptr<ActivationFunction> LinearFunction::clone() const {
    return std::unique_ptr<ActivationFunction>(new LinearFunction());
}

// Factory function
std::unique_ptr<ActivationFunction> createActivationFunction(const std::string& name) {
    if (name == "Sigmoid") {
        return std::unique_ptr<ActivationFunction>(new SigmoidFunction());
    }
    else if (name == "Tanh") {
        return std::unique_ptr<ActivationFunction>(new TanhFunction());
    }
    else if (name == "ReLU") {
        return std::unique_ptr<ActivationFunction>(new ReLUFunction());
    }
    else if (name == "Linear") {
        return std::unique_ptr<ActivationFunction>(new LinearFunction());
    }
    
    return nullptr; // Unknown activation function
}
