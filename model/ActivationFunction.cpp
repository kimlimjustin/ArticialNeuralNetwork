//-------------------------------------------------------------
//【文件名】ActivationFunction.cpp
//【功能模块和目的】激活函数工厂函数实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "ActivationFunction.hpp"
#include "SigmoidFunction.hpp"
#include "TanhFunction.hpp"
#include "ReLUFunction.hpp"
#include "LinearFunction.hpp"

using namespace std;

//-------------------------------------------------------------
//【函数名称】createActivationFunction
//【函数功能】根据名称创建激活函数对象
//【参数】name：激活函数名称
//【返回值】unique_ptr<ActivationFunction>，激活函数对象
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
unique_ptr<ActivationFunction> createActivationFunction(const string& name) {
    if (name == "Sigmoid") {
        return unique_ptr<ActivationFunction>(new SigmoidFunction());
    }
    else if (name == "Tanh") {
        return unique_ptr<ActivationFunction>(new TanhFunction());
    }
    else if (name == "ReLU") {
        return unique_ptr<ActivationFunction>(new ReLUFunction());
    }
    else if (name == "Linear") {
        return unique_ptr<ActivationFunction>(new LinearFunction());
    }
    
    return nullptr; // Unknown activation function
}
