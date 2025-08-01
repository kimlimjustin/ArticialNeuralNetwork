//-------------------------------------------------------------
//【文件名】Synapse.cpp
//【功能模块和目的】突触类实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "Synapse.hpp"
#include "Neuron.hpp"
#include <stdexcept>

//-------------------------------------------------------------
//【函数名称】Synapse
//【函数功能】突触构造函数
//【参数】weight：权重，sourceNeuron：源神经元，targetNeuron：目标神经元，isAxon：是否为轴突
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Synapse::Synapse(double weight, Neuron* sourceNeuron, Neuron* targetNeuron, bool isAxon)
    : m_sourceNeuron(sourceNeuron), m_targetNeuron(targetNeuron), m_isAxon(isAxon) {
    
    if (isAxon) {
        // 根据规范：轴突权重恒为1.0
        m_weight = 1.0;
        if (sourceNeuron == nullptr) {
            throw std::invalid_argument("Axon must have a source neuron");
        }
        // Target can be null for external output
    } else {
        // 树突：权重可为任意实数，存储实际的连接权重
        m_weight = weight;
        if (targetNeuron == nullptr) {
            throw std::invalid_argument("Dendrite must have a target neuron");
        }
        // Source can be null for external input
    }
}

//-------------------------------------------------------------
//【函数名称】Synapse（拷贝构造）
//【函数功能】拷贝构造函数
//【参数】other：被拷贝的突触
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Synapse::Synapse(const Synapse& other)
    : m_weight(other.m_isAxon ? 1.0 : other.m_weight), m_sourceNeuron(other.m_sourceNeuron),
      m_targetNeuron(other.m_targetNeuron), m_isAxon(other.m_isAxon) {
}

//-------------------------------------------------------------
//【函数名称】operator=
//【函数功能】赋值运算符重载
//【参数】other：赋值来源突触
//【返回值】Synapse&，自身引用
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Synapse& Synapse::operator=(const Synapse& other) {
    if (this != &other) {
        m_weight = other.m_isAxon ? 1.0 : other.m_weight;
        m_sourceNeuron = other.m_sourceNeuron;
        m_targetNeuron = other.m_targetNeuron;
        m_isAxon = other.m_isAxon;
    }
    return *this;
}

//-------------------------------------------------------------
//【函数名称】~Synapse
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Synapse::~Synapse() = default;

//-------------------------------------------------------------
//【函数名称】getWeight
//【函数功能】获取权重
//【参数】无
//【返回值】权重值
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double Synapse::getWeight() const {
    return m_weight;
}

//-------------------------------------------------------------
//【函数名称】setWeight
//【函数功能】设置权重
//【参数】weight：权重值
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Synapse::setWeight(double weight) {
    if (!m_isAxon) {
        m_weight = weight;
    }
    // Axon weights are always 1.0 and cannot be changed
}

//-------------------------------------------------------------
//【函数名称】getSourceNeuron
//【函数功能】获取源神经元
//【参数】无
//【返回值】源神经元指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron* Synapse::getSourceNeuron() const {
    return m_sourceNeuron;
}

//-------------------------------------------------------------
//【函数名称】getTargetNeuron
//【函数功能】获取目标神经元
//【参数】无
//【返回值】目标神经元指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron* Synapse::getTargetNeuron() const {
    return m_targetNeuron;
}

//-------------------------------------------------------------
//【函数名称】setSourceNeuron
//【函数功能】设置源神经元
//【参数】neuron：神经元指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Synapse::setSourceNeuron(Neuron* neuron) {
    m_sourceNeuron = neuron;
}

//-------------------------------------------------------------
//【函数名称】setTargetNeuron
//【函数功能】设置目标神经元
//【参数】neuron：神经元指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Synapse::setTargetNeuron(Neuron* neuron) {
    if (!m_isAxon && neuron == nullptr) {
        throw std::invalid_argument("Dendrite target neuron cannot be null");
    }
    // Axons can have null target for external output
    m_targetNeuron = neuron;
}

//-------------------------------------------------------------
//【函数名称】isAxon
//【函数功能】判断是否为轴突
//【参数】无
//【返回值】true：是轴突，false：不是轴突
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Synapse::isAxon() const {
    return m_isAxon;
}

//-------------------------------------------------------------
//【函数名称】transmit
//【函数功能】信号传递
//【参数】input：输入信号
//【返回值】输出信号
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double Synapse::transmit(double input) const {
    return input * m_weight;
}

//-------------------------------------------------------------
//【函数名称】isValid
//【函数功能】有效性检查
//【参数】无
//【返回值】true：有效，false：无效
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Synapse::isValid() const {
    // For axons (outgoing connections): weight must be 1.0
    if (m_isAxon) {
        if (m_weight != 1.0) {
            return false;
        }
        // Axons represent output from source neuron
        // Source neuron should exist (the neuron this axon belongs to)
        // Target can be null for external output connections
        return m_sourceNeuron != nullptr;
    }
    
    // For dendrites (incoming connections): weight can be any real number
    // Target neuron should exist (the neuron this dendrite belongs to)
    // Source can be null for external input connections
    return m_targetNeuron != nullptr;
}
