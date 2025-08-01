//-------------------------------------------------------------
//【文件名】SigmoidFunction.hpp
//【功能模块和目的】Sigmoid激活函数声明
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#ifndef SigmoidFunction_hpp
#define SigmoidFunction_hpp

#include "ActivationFunction.hpp"

using namespace std;

//-------------------------------------------------------------
//【类名】SigmoidFunction
//【功能】Sigmoid 激活函数实现


//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
class SigmoidFunction : public ActivationFunction {
public:
    //-------------------------------------------------------------
    //【函数名称】SigmoidFunction
    //【函数功能】默认构造函数
    //【参数】无
    //【返回值】无
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    SigmoidFunction() = default;
    
    //-------------------------------------------------------------
    //【函数名称】SigmoidFunction（拷贝构造）
    //【函数功能】拷贝构造函数
    //【参数】other：被拷贝的Sigmoid函数
    //【返回值】无
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    SigmoidFunction(const SigmoidFunction& other) = default;
    
    //-------------------------------------------------------------
    //【函数名称】operator=
    //【函数功能】赋值运算符重载
    //【参数】other：赋值来源Sigmoid函数
    //【返回值】SigmoidFunction&，自身引用
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    SigmoidFunction& operator=(const SigmoidFunction& other) = default;
    
    //-------------------------------------------------------------
    //【函数名称】~SigmoidFunction
    //【函数功能】析构函数
    //【参数】无
    //【返回值】无
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    ~SigmoidFunction() = default;
    
    //-------------------------------------------------------------
    //【函数名称】activate
    //【函数功能】Sigmoid 激活变换
    //【参数】x：输入值
    //【返回值】double，Sigmoid 输出（0到1之间）
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    double activate(double x) const override;
    
    //-------------------------------------------------------------
    //【函数名称】getName
    //【函数功能】获取函数名称
    //【参数】无
    //【返回值】string，函数名"Sigmoid"
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    string getName() const override;
    
    //-------------------------------------------------------------
    //【函数名称】clone
    //【函数功能】克隆函数对象
    //【参数】无
    //【返回值】unique_ptr<ActivationFunction>，克隆对象
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    unique_ptr<ActivationFunction> clone() const override;
};

#endif // SigmoidFunction_hpp
