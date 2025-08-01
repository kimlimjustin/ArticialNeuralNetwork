//-------------------------------------------------------------
//【文件名】TanhFunction.cpp
//【功能模块和目的】Tanh激活函数实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "TanhFunction.hpp"
#include <cmath>

using namespace std;

//-------------------------------------------------------------
//【函数名称】activate
//【函数功能】Tanh 激活函数实现
//【参数】x：输入值
//【返回值】double，激活后输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double TanhFunction::activate(double x) const {
    // f(x) = (e^x - e^(-x)) / (e^x + e^(-x)) as per specification
    double ex = exp(x);
    double e_neg_x = exp(-x);
    return (ex - e_neg_x) / (ex + e_neg_x);
}

//-------------------------------------------------------------
//【函数名称】getName
//【函数功能】获取函数名称
//【参数】无
//【返回值】string，函数名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string TanhFunction::getName() const {
    return "Tanh";
}

//-------------------------------------------------------------
//【函数名称】clone
//【函数功能】克隆自身
//【参数】无
//【返回值】unique_ptr<ActivationFunction>
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
unique_ptr<ActivationFunction> TanhFunction::clone() const {
    return unique_ptr<ActivationFunction>(new TanhFunction());
}
