//-------------------------------------------------------------
//【文件名】Neuron.cpp
//【功能模块和目的】神经元类实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "Neuron.hpp"
#include <algorithm>
#include <stdexcept>

using namespace std;

//-------------------------------------------------------------
//【函数名称】Neuron
//【函数功能】神经元构造函数
//【参数】bias：偏置，activationFunction：激活函数指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron::Neuron(double bias, unique_ptr<ActivationFunction> activationFunction)
    : m_bias(bias), m_activationFunction(move(activationFunction)),
      m_lastOutput(0.0), m_hasComputedOutput(false) {
}

//-------------------------------------------------------------
//【函数名称】Neuron（拷贝构造）
//【函数功能】拷贝构造函数
//【参数】other：被拷贝的神经元
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron::Neuron(const Neuron& other)
    : m_bias(other.m_bias), m_lastOutput(other.m_lastOutput),
      m_hasComputedOutput(other.m_hasComputedOutput) {
    // Deep copy activation function
    if (other.m_activationFunction) {
        m_activationFunction = other.m_activationFunction->clone();
    }
    
    // Deep copy input synapses
    for (const auto& synapse : other.m_inputSynapses) {
        m_inputSynapses.push_back(unique_ptr<Synapse>(new Synapse(*synapse)));
    }
    
    // Deep copy output synapses
    for (const auto& synapse : other.m_outputSynapses) {
        m_outputSynapses.push_back(unique_ptr<Synapse>(new Synapse(*synapse)));
    }
}

//-------------------------------------------------------------
//【函数名称】operator=
//【函数功能】赋值运算符重载
//【参数】other：赋值来源神经元
//【返回值】Neuron&，自身引用
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron& Neuron::operator=(const Neuron& other) {
    if (this != &other) {
        m_bias = other.m_bias;
        m_lastOutput = other.m_lastOutput;
        m_hasComputedOutput = other.m_hasComputedOutput;
        
        // Clear existing synapses
        m_inputSynapses.clear();
        m_outputSynapses.clear();
        
        // Deep copy activation function
        if (other.m_activationFunction) {
            m_activationFunction = other.m_activationFunction->clone();
        } else {
            m_activationFunction = nullptr;
        }
        
        // Deep copy input synapses
        for (const auto& synapse : other.m_inputSynapses) {
            m_inputSynapses.push_back(unique_ptr<Synapse>(new Synapse(*synapse)));
        }
        
        // Deep copy output synapses
        for (const auto& synapse : other.m_outputSynapses) {
            m_outputSynapses.push_back(unique_ptr<Synapse>(new Synapse(*synapse)));
        }
    }
    return *this;
}

//-------------------------------------------------------------
//【函数名称】~Neuron
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron::~Neuron() = default;

//-------------------------------------------------------------
//【函数名称】getBias
//【函数功能】获取神经元偏置
//【参数】无
//【返回值】double，偏置值
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double Neuron::getBias() const {
    return m_bias;
}

//-------------------------------------------------------------
//【函数名称】setBias
//【函数功能】设置神经元偏置
//【参数】bias：新偏置
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Neuron::setBias(double bias) {
    m_bias = bias;
    m_hasComputedOutput = false; // Invalidate cached output
}

//-------------------------------------------------------------
//【函数名称】getActivationFunction
//【函数功能】获取激活函数指针
//【参数】无
//【返回值】const ActivationFunction*，激活函数指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const ActivationFunction* Neuron::getActivationFunction() const {
    return m_activationFunction.get();
}

//-------------------------------------------------------------
//【函数名称】setActivationFunction
//【函数功能】设置激活函数
//【参数】activationFunction：激活函数指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Neuron::setActivationFunction(unique_ptr<ActivationFunction> activationFunction) {
    m_activationFunction = move(activationFunction);
    m_hasComputedOutput = false; // Invalidate cached output
}

//-------------------------------------------------------------
//【函数名称】addInputSynapse
//【函数功能】添加输入突触
//【参数】synapse：输入突触指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Neuron::addInputSynapse(unique_ptr<Synapse> synapse) {
    if (synapse) {
        synapse->setTargetNeuron(this);
        m_inputSynapses.push_back(move(synapse));
        m_hasComputedOutput = false; // Invalidate cached output
    }
}

//-------------------------------------------------------------
//【函数名称】addOutputSynapse
//【函数功能】添加输出突触
//【参数】synapse：输出突触指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Neuron::addOutputSynapse(unique_ptr<Synapse> synapse) {
    if (synapse) {
        synapse->setSourceNeuron(this);
        m_outputSynapses.push_back(move(synapse));
    }
}

//-------------------------------------------------------------
//【函数名称】getInputSynapseCount
//【函数功能】获取输入突触数量
//【参数】无
//【返回值】int，输入突触数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Neuron::getInputSynapseCount() const {
    return static_cast<int>(m_inputSynapses.size());
}

//-------------------------------------------------------------
//【函数名称】getOutputSynapseCount
//【函数功能】获取输出突触数量
//【参数】无
//【返回值】int，输出突触数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Neuron::getOutputSynapseCount() const {
    return static_cast<int>(m_outputSynapses.size());
}

//-------------------------------------------------------------
//【函数名称】getInputSynapse
//【函数功能】获取指定输入突触指针
//【参数】index：索引
//【返回值】const Synapse*，输入突触指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const Synapse* Neuron::getInputSynapse(int index) const {
    if (index >= 0 && index < static_cast<int>(m_inputSynapses.size())) {
        return m_inputSynapses[index].get();
    }
    return nullptr;
}

//-------------------------------------------------------------
//【函数名称】getOutputSynapse
//【函数功能】获取指定输出突触指针
//【参数】index：索引
//【返回值】const Synapse*，输出突触指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const Synapse* Neuron::getOutputSynapse(int index) const {
    if (index >= 0 && index < static_cast<int>(m_outputSynapses.size())) {
        return m_outputSynapses[index].get();
    }
    return nullptr;
}

//-------------------------------------------------------------
//【函数名称】removeInputSynapse
//【函数功能】移除指定输入突触
//【参数】index：索引
//【返回值】bool，是否移除成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Neuron::removeInputSynapse(int index) {
    if (index >= 0 && index < static_cast<int>(m_inputSynapses.size())) {
        m_inputSynapses.erase(m_inputSynapses.begin() + index);
        m_hasComputedOutput = false; // Invalidate cached output
        return true;
    }
    return false;
}

//-------------------------------------------------------------
//【函数名称】removeOutputSynapse
//【函数功能】移除指定输出突触
//【参数】index：索引
//【返回值】bool，是否移除成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Neuron::removeOutputSynapse(int index) {
    if (index >= 0 && index < static_cast<int>(m_outputSynapses.size())) {
        m_outputSynapses.erase(m_outputSynapses.begin() + index);
        return true;
    }
    return false;
}

//-------------------------------------------------------------
//【函数名称】connectTo
//【函数功能】连接到目标神经元，根据规范轴突权重恒为1.0，连接权重存储在树突中
//【参数】targetNeuron：目标神经元，weight：连接权重
//【返回值】bool，是否连接成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Neuron::connectTo(Neuron& targetNeuron, double weight) {
    // 根据规范：轴突权重恒为1.0，连接的实际权重存储在目标神经元的树突中
    
    // Create output synapse (axon) for this neuron with weight 1.0
    unique_ptr<Synapse> outputSynapse(new Synapse(1.0, this, &targetNeuron, true));
    
    // Create input synapse (dendrite) for target neuron with the actual connection weight  
    unique_ptr<Synapse> inputSynapse(new Synapse(weight, this, &targetNeuron, false));
    
    // Add synapses to respective neurons
    this->addOutputSynapse(move(outputSynapse));
    targetNeuron.addInputSynapse(move(inputSynapse));
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】computeOutput
//【函数功能】计算神经元输出
//【参数】inputs：输入信号集合
//【返回值】double，输出值
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double Neuron::computeOutput(const vector<double>& inputs) {
    if (inputs.size() != m_inputSynapses.size()) {
        throw runtime_error("Input size mismatch with number of input synapses");
    }
    
    // Sum function: bias + sum of weighted inputs
    double rSum = m_bias;
    for (size_t uInputIdx = 0; uInputIdx < inputs.size(); ++uInputIdx) {
        rSum += m_inputSynapses[uInputIdx]->transmit(inputs[uInputIdx]);
    }
    
    // Activation function
    if (m_activationFunction) {
        m_lastOutput = m_activationFunction->activate(rSum);
    } else {
        // Linear activation (f(x) = x)
        m_lastOutput = rSum;
    }
    
    m_hasComputedOutput = true;
    return m_lastOutput;
}

//-------------------------------------------------------------
//【函数名称】getLastOutput
//【函数功能】获取上一次输出值
//【参数】无
//【返回值】double，上一次输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double Neuron::getLastOutput() const {
    return m_lastOutput;
}

//-------------------------------------------------------------
//【函数名称】hasValidOutput
//【函数功能】判断输出值是否有效
//【参数】无
//【返回值】bool，输出是否有效
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Neuron::hasValidOutput() const {
    return m_hasComputedOutput;
}

//-------------------------------------------------------------
//【函数名称】resetComputationState
//【函数功能】重置计算状态
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Neuron::resetComputationState() {
    m_hasComputedOutput = false;
    m_lastOutput = 0.0;
}

//-------------------------------------------------------------
//【函数名称】isValid
//【函数功能】判断神经元结构是否合法
//【参数】无
//【返回值】bool，结构是否合法
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Neuron::isValid() const {
    // Check that all input synapses have valid weights and connections
    for (const auto& synapse : m_inputSynapses) {
        if (!synapse || !synapse->isValid()) {
            return false;
        }
    }
    
    // Check that all output synapses have valid weights and connections
    for (const auto& synapse : m_outputSynapses) {
        if (!synapse || !synapse->isValid()) {
            return false;
        }
    }
    
    // Check that activation function is properly set (if needed)
    // Note: nullptr activation function is valid (means linear activation)
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】disconnectAll
//【函数功能】断开所有突触连接
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Neuron::disconnectAll() {
    m_inputSynapses.clear();
    m_outputSynapses.clear();
    m_hasComputedOutput = false;
}
