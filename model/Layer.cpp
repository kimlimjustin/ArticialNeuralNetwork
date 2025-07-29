//-------------------------------------------------------------
//【文件名】Layer.cpp
//【功能模块和目的】神经网络层类实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "Layer.hpp"
#include <stdexcept>
#include <algorithm>

using namespace std;

//-------------------------------------------------------------
//【函数名称】Layer
//【函数功能】默认构造函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Layer::Layer() {
}

//-------------------------------------------------------------
//【函数名称】Layer
//【函数功能】带初始神经元数量的构造函数
//【参数】neuronCount：神经元数量，bias：默认偏置，activationFunction：激活函数
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Layer::Layer(int neuronCount, double bias, unique_ptr<ActivationFunction> activationFunction) {
    for (int i = 0; i < neuronCount; ++i) {
        unique_ptr<ActivationFunction> neuronActivation = nullptr;
        if (activationFunction) {
            neuronActivation = activationFunction->clone();
        }
        m_neurons.push_back(unique_ptr<Neuron>(new Neuron(bias, move(neuronActivation))));
    }
}

//-------------------------------------------------------------
//【函数名称】Layer（拷贝构造）
//【函数功能】拷贝构造函数
//【参数】other：被拷贝的层
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Layer::Layer(const Layer& other) {
    for (const auto& neuron : other.m_neurons) {
        m_neurons.push_back(unique_ptr<Neuron>(new Neuron(*neuron)));
    }
}

//-------------------------------------------------------------
//【函数名称】operator=
//【函数功能】赋值运算符重载
//【参数】other：赋值来源层
//【返回值】Layer&，自身引用
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Layer& Layer::operator=(const Layer& other) {
    if (this != &other) {
        m_neurons.clear();
        for (const auto& neuron : other.m_neurons) {
            m_neurons.push_back(unique_ptr<Neuron>(new Neuron(*neuron)));
        }
    }
    return *this;
}

//-------------------------------------------------------------
//【函数名称】~Layer
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Layer::~Layer() {
}

//-------------------------------------------------------------
//【函数名称】addNeuron
//【函数功能】添加神经元
//【参数】neuron：神经元指针
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Layer::addNeuron(unique_ptr<Neuron> neuron) {
    if (neuron) {
        m_neurons.push_back(move(neuron));
    }
}

//-------------------------------------------------------------
//【函数名称】removeNeuron
//【函数功能】移除指定索引的神经元
//【参数】index：神经元索引
//【返回值】bool，是否移除成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Layer::removeNeuron(int index) {
    if (index >= 0 && index < static_cast<int>(m_neurons.size())) {
        // Disconnect the neuron before removing it
        m_neurons[index]->disconnectAll();
        m_neurons.erase(m_neurons.begin() + index);
        return true;
    }
    return false;
}

//-------------------------------------------------------------
//【函数名称】getNeuron
//【函数功能】获取指定索引的神经元指针
//【参数】index：神经元索引
//【返回值】Neuron*，神经元指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron* Layer::getNeuron(int index) {
    if (index >= 0 && index < static_cast<int>(m_neurons.size())) {
        return m_neurons[index].get();
    }
    return nullptr;
}

//-------------------------------------------------------------
//【函数名称】getNeuron（常量版本）
//【函数功能】获取指定索引的神经元指针（常量）
//【参数】index：神经元索引
//【返回值】const Neuron*，神经元指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const Neuron* Layer::getNeuron(int index) const {
    if (index >= 0 && index < static_cast<int>(m_neurons.size())) {
        return m_neurons[index].get();
    }
    return nullptr;
}

//-------------------------------------------------------------
//【函数名称】getNeuronCount
//【函数功能】获取神经元数量
//【参数】无
//【返回值】int，神经元数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Layer::getNeuronCount() const {
    return static_cast<int>(m_neurons.size());
}

//-------------------------------------------------------------
//【函数名称】forwardPropagate
//【函数功能】前向传播（每个神经元独立输入）
//【参数】inputs：每个神经元的输入集合
//【返回值】vector<double>，所有神经元输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
vector<double> Layer::forwardPropagate(const vector<vector<double>>& inputs) {
    if (inputs.size() != m_neurons.size()) {
        throw runtime_error("Input size mismatch with number of neurons");
    }
    
    vector<double> outputs;
    outputs.reserve(m_neurons.size());
    
    for (size_t i = 0; i < m_neurons.size(); ++i) {
        outputs.push_back(m_neurons[i]->computeOutput(inputs[i]));
    }
    
    return outputs;
}

//-------------------------------------------------------------
//【函数名称】forwardPropagateUniform
//【函数功能】前向传播（所有神经元共用输入）
//【参数】inputs：统一输入集合
//【返回值】vector<double>，所有神经元输出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
vector<double> Layer::forwardPropagateUniform(const vector<double>& inputs) {
    vector<double> outputs;
    outputs.reserve(m_neurons.size());
    
    for (auto& neuron : m_neurons) {
        outputs.push_back(neuron->computeOutput(inputs));
    }
    
    return outputs;
}

//-------------------------------------------------------------
//【函数名称】resetComputationState
//【函数功能】重置所有神经元的计算状态
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Layer::resetComputationState() {
    for (auto& neuron : m_neurons) {
        neuron->resetComputationState();
    }
}

//-------------------------------------------------------------
//【函数名称】isValid
//【函数功能】判断层结构是否合法
//【参数】无
//【返回值】bool，结构是否合法
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Layer::isValid() const {
    if (m_neurons.empty()) {
        return false;
    }
    
    for (const auto& neuron : m_neurons) {
        if (!neuron->isValid()) {
            return false;
        }
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】clear
//【函数功能】清空所有神经元
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Layer::clear() {
    m_neurons.clear();
}

//-------------------------------------------------------------
//【函数名称】getTotalSynapseCount
//【函数功能】获取本层所有突触数量
//【参数】无
//【返回值】int，突触数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Layer::getTotalSynapseCount() const {
    int totalSynapses = 0;
    for (const auto& neuron : m_neurons) {
        totalSynapses += neuron->getInputSynapseCount();
        totalSynapses += neuron->getOutputSynapseCount();
    }
    return totalSynapses;
}

//-------------------------------------------------------------
//【函数名称】connectToLayer
//【函数功能】连接到目标层
//【参数】targetLayer：目标层，weights：连接权重（可选）
//【返回值】bool，是否连接成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Layer::connectToLayer(Layer& targetLayer, const vector<vector<double>>& weights) {
    if (targetLayer.getNeuronCount() == 0) {
        return false;
    }
    
    bool useProvidedWeights = !weights.empty() && 
                             weights.size() == static_cast<size_t>(getNeuronCount()) &&
                             weights[0].size() == static_cast<size_t>(targetLayer.getNeuronCount());
    
    for (int i = 0; i < getNeuronCount(); ++i) {
        for (int j = 0; j < targetLayer.getNeuronCount(); ++j) {
            double weight = useProvidedWeights ? weights[i][j] : 1.0; // Default weight
            
            Neuron* targetNeuron = targetLayer.getNeuron(j);
            if (!targetNeuron) {
                return false; // Invalid target neuron
            }
            
            if (!m_neurons[i]->connectTo(*targetNeuron, weight)) {
                return false;
            }
        }
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】disconnectAll
//【函数功能】断开本层所有神经元的连接
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Layer::disconnectAll() {
    for (auto& neuron : m_neurons) {
        neuron->disconnectAll();
    }
}
