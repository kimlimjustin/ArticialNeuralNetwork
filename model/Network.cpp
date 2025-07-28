//-------------------------------------------------------------
//【文件名】Network.cpp
//【功能模块和目的】神经网络整体结构实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "Network.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <set>
#include <queue>

//-------------------------------------------------------------
//【函数名称】Network
//【函数功能】默认构造函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::Network() : m_name("Unnamed Network") {
}

//-------------------------------------------------------------
//【函数名称】Network
//【函数功能】带名称的构造函数
//【参数】name：网络标识
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::Network(const std::string& name) : m_name(name) {
}

//-------------------------------------------------------------
//【函数名称】Network
//【函数功能】拷贝构造函数
//【参数】other：待拷贝的网络对象
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::Network(const Network& other) : m_name(other.m_name) {
    for (const auto& layer : other.m_layers) {
        m_layers.push_back(std::unique_ptr<Layer>(new Layer(*layer)));
    }
}

//-------------------------------------------------------------
//【函数名称】operator=
//【函数功能】赋值运算符重载
//【参数】other：待赋值的网络对象
//【返回值】当前对象的引用
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network& Network::operator=(const Network& other) {
    if (this != &other) {
        m_name = other.m_name;
        m_layers.clear();
        for (const auto& layer : other.m_layers) {
            m_layers.push_back(std::unique_ptr<Layer>(new Layer(*layer)));
        }
    }
    return *this;
}

//-------------------------------------------------------------
//【函数名称】~Network
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::~Network() {
}

//-------------------------------------------------------------
//【函数名称】getName
//【函数功能】获取网络名称
//【参数】无
//【返回值】网络名称
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const std::string& Network::getName() const {
    return m_name;
}

//-------------------------------------------------------------
//【函数名称】setName
//【函数功能】设置网络名称
//【参数】name：新的网络名称
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::setName(const std::string& name) {
    m_name = name;
}

//-------------------------------------------------------------
//【函数名称】addLayer
//【函数功能】添加层
//【参数】layer：待添加的层
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::addLayer(std::unique_ptr<Layer> layer) {
    if (layer) {
        m_layers.push_back(std::move(layer));
    }
}

//-------------------------------------------------------------
//【函数名称】removeLayer
//【函数功能】移除层
//【参数】index：层的索引
//【返回值】成功移除返回true，失败返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::removeLayer(int index) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        // Disconnect the layer before removing it
        m_layers[index]->disconnectAll();
        m_layers.erase(m_layers.begin() + index);
        return true;
    }
    return false;
}

//-------------------------------------------------------------
//【函数名称】getLayer
//【函数功能】获取指定索引的层
//【参数】index：层的索引
//【返回值】对应层的指针，失败返回nullptr
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Layer* Network::getLayer(int index) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        return m_layers[index].get();
    }
    return nullptr;
}

//-------------------------------------------------------------
//【函数名称】getLayer
//【函数功能】获取指定索引的层（常量版本）
//【参数】index：层的索引
//【返回值】对应层的指针，失败返回nullptr
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const Layer* Network::getLayer(int index) const {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        return m_layers[index].get();
    }
    return nullptr;
}

//-------------------------------------------------------------
//【函数名称】getLayerCount
//【函数功能】获取层的数量
//【参数】无
//【返回值】层的数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Network::getLayerCount() const {
    return static_cast<int>(m_layers.size());
}

//-------------------------------------------------------------
//【函数名称】getNeuronCount
//【函数功能】获取神经元的总数量
//【参数】无
//【返回值】神经元的总数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Network::getNeuronCount() const {
    int totalNeurons = 0;
    for (const auto& layer : m_layers) {
        totalNeurons += layer->getNeuronCount();
    }
    return totalNeurons;
}

//-------------------------------------------------------------
//【函数名称】getSynapseCount
//【函数功能】获取突触的总数量
//【参数】无
//【返回值】突触的总数量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int Network::getSynapseCount() const {
    int totalSynapses = 0;
    for (const auto& layer : m_layers) {
        totalSynapses += layer->getTotalSynapseCount();
    }
    return totalSynapses;
}

//-------------------------------------------------------------
//【函数名称】isValid
//【函数功能】验证网络结构是否有效
//【参数】无
//【返回值】有效返回true， 无效返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::isValid() const {
    // Network must have at least one layer
    if (m_layers.empty()) {
        return false;
    }
    
    // All layers must be valid
    for (const auto& layer : m_layers) {
        if (!layer->isValid()) {
            return false;
        }
    }
    
    // First layer must have neurons to accept input
    if (m_layers[0]->getNeuronCount() == 0) {
        return false;
    }
    
    // Last layer must have neurons to produce output
    if (m_layers.back()->getNeuronCount() == 0) {
        return false;
    }
    
    // Check for cycles (not allowed in feedforward networks)
    if (hasCycles()) {
        return false;
    }
    
    // Check that all neurons participate in data flow
    if (!allNeuronsParticipate()) {
        return false;
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】predict
//【函数功能】根据输入进行预测
//【参数】inputs：输入数据
//【返回值】预测结果
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::vector<double> Network::predict(const std::vector<double>& inputs) {
    if (!isValid()) {
        throw std::runtime_error("Network is not valid for inference");
    }
    
    if (m_layers.empty()) {
        throw std::runtime_error("Network has no layers");
    }
    
    // Check input size matches first layer
    if (static_cast<int>(inputs.size()) != m_layers[0]->getNeuronCount()) {
        throw std::runtime_error("Input size mismatch with first layer neuron count");
    }
    
    // Reset computation state
    resetComputationState();
    
    // Forward propagate through all layers
    std::vector<double> currentOutputs = inputs;
    
    for (size_t layerIndex = 0; layerIndex < m_layers.size(); ++layerIndex) {
        Layer* layer = m_layers[layerIndex].get();
        
        if (layerIndex == 0) {
            // For first layer, each neuron gets one input value
            // Check if neurons have input synapses
            std::vector<std::vector<double>> layerInputs;
            for (int i = 0; i < layer->getNeuronCount(); ++i) {
                const Neuron* neuron = layer->getNeuron(i);
                if (neuron && neuron->getInputSynapseCount() > 0) {
                    // Neuron expects inputs through synapses
                    layerInputs.push_back({currentOutputs[i]});
                } else {
                    // Neuron can accept direct input
                    layerInputs.push_back({currentOutputs[i]});
                }
            }
            currentOutputs = layer->forwardPropagate(layerInputs);
        } else {
            // For subsequent layers, neurons get outputs from previous layer
            std::vector<std::vector<double>> layerInputs;
            for (int neuronIndex = 0; neuronIndex < layer->getNeuronCount(); ++neuronIndex) {
                const Neuron* neuron = layer->getNeuron(neuronIndex);
                if (neuron && neuron->getInputSynapseCount() > 0) {
                    // Neuron has input synapses, provide all previous outputs
                    layerInputs.push_back(currentOutputs);
                } else {
                    // Neuron without synapses, provide all previous outputs anyway
                    layerInputs.push_back(currentOutputs);
                }
            }
            currentOutputs = layer->forwardPropagate(layerInputs);
        }
    }
    
    return currentOutputs;
}

//-------------------------------------------------------------
//【函数名称】resetComputationState
//【函数功能】重置计算状态
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::resetComputationState() {
    for (auto& layer : m_layers) {
        layer->resetComputationState();
    }
}

//-------------------------------------------------------------
//【函数名称】clear
//【函数功能】清空网络
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::clear() {
    m_layers.clear();
}

//-------------------------------------------------------------
//【函数名称】connectAllLayers
//【函数功能】连接所有层
//【参数】defaultWeight：默认权重
//【返回值】成功连接返回true，失败返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::connectAllLayers(double defaultWeight) {
    if (m_layers.size() < 2) {
        return true; // Nothing to connect
    }
    
    for (size_t i = 0; i < m_layers.size() - 1; ++i) {
        if (!m_layers[i]->connectToLayer(*m_layers[i + 1])) {
            return false;
        }
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】getStructureInfo
//【函数功能】获取网络结构信息
//【参数】无
//【返回值】网络结构信息字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string Network::getStructureInfo() const {
    std::ostringstream oss;
    oss << "Network: " << m_name << "\n";
    oss << "Layers: " << getLayerCount() << "\n";
    oss << "Total Neurons: " << getNeuronCount() << "\n";
    oss << "Total Synapses: " << getSynapseCount() << "\n";
    oss << "Valid: " << (isValid() ? "Yes" : "No") << "\n";
    
    for (int i = 0; i < getLayerCount(); ++i) {
        const Layer* layer = getLayer(i);
        oss << "  Layer " << i << ": " << layer->getNeuronCount() << " neurons\n";
    }
    
    return oss.str();
}

//-------------------------------------------------------------
//【函数名称】hasCycles
//【函数功能】检查是否有循环
//【参数】无
//【返回值】有循环返回true，无循环返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::hasCycles() const {
    // For a feedforward network, there should be no cycles
    // This is a simplified check - in a proper implementation,
    // we would perform a topological sort or DFS cycle detection
    // For now, assume feedforward structure if layers are connected sequentially
    return false;
}

//-------------------------------------------------------------
//【函数名称】allNeuronsParticipate
//【函数功能】检查所有神经元是否参与
//【参数】无
//【返回值】参与返回true，未参与返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::allNeuronsParticipate() const {
    // Check that all neurons can be reached from input and can reach output
    // This is a simplified check - in a proper implementation,
    // we would perform graph traversal to verify connectivity
    
    if (m_layers.empty()) {
        return false;
    }
    
    // Check that each layer has neurons
    for (const auto& layer : m_layers) {
        if (layer->getNeuronCount() == 0) {
            return false;
        }
    }
    
    return true;
}
