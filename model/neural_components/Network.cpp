//-------------------------------------------------------------
//【文件名】Network.cpp
//【功能模块和目的】神经网络整体结构实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "Network.hpp"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <set>
#include <queue>
#include <map>
#include <functional>

using namespace std;

//-------------------------------------------------------------
//【函数名称】Network
//【函数功能】默认构造函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::Network() : m_name("Unnamed Network"), 
                 m_hasImportErrors(false), m_importErrorMessage(""), m_validationCacheValid(false) {
}

//-------------------------------------------------------------
//【函数名称】Network
//【函数功能】带名称的构造函数
//【参数】name：网络标识
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::Network(const string& name) : m_name(name), 
                                  m_hasImportErrors(false), m_importErrorMessage(""), m_validationCacheValid(false) {
}

//-------------------------------------------------------------
//【函数名称】Network
//【函数功能】拷贝构造函数
//【参数】other：待拷贝的网络对象
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Network::Network(const Network& other) : m_name(other.m_name), 
                                   m_hasImportErrors(other.m_hasImportErrors), 
                                   m_importErrorMessage(other.m_importErrorMessage),
                                   m_validationCacheValid(other.m_validationCacheValid) {
    for (const auto& layer : other.m_layers) {
        m_layers.push_back(unique_ptr<Layer>(new Layer(*layer)));
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
        m_hasImportErrors = other.m_hasImportErrors;
        m_importErrorMessage = other.m_importErrorMessage;
        m_validationCacheValid = other.m_validationCacheValid;
        m_layers.clear();
        for (const auto& layer : other.m_layers) {
            m_layers.push_back(unique_ptr<Layer>(new Layer(*layer)));
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
Network::~Network() = default;

//-------------------------------------------------------------
//【函数名称】getName
//【函数功能】获取网络名称
//【参数】无
//【返回值】网络名称
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
const string& Network::getName() const {
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
void Network::setName(const string& name) {
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
void Network::addLayer(unique_ptr<Layer> layer) {
    if (layer) {
        m_layers.push_back(move(layer));
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
    int iTotalNeurons = 0;
    for (const auto& layer : m_layers) {
        iTotalNeurons += layer->getNeuronCount();
    }
    return iTotalNeurons;
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
    int iTotalSynapses = 0;
    for (const auto& layer : m_layers) {
        iTotalSynapses += layer->getTotalSynapseCount();
    }
    return iTotalSynapses;
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
    // Check for cached import errors first
    if (m_hasImportErrors) {
        return false;
    }
    
    // Network must have at least one layer
    if (m_layers.empty()) {
        return false;
    }
    
    // All layers must have at least one neuron
    for (const auto& layer : m_layers) {
        if (layer->getNeuronCount() == 0) {
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
    
    // 根据规范检查所有轴突权重是否为1.0
    for (const auto& layer : m_layers) {
        for (int iNeuronIdx = 0; iNeuronIdx < layer->getNeuronCount(); ++iNeuronIdx) {
            const Neuron* pNeuron = layer->getNeuron(iNeuronIdx);
            if (!pNeuron) continue;
            
            // Check all output synapses (axons) have weight 1.0
            for (int iSynapseIdx = 0; iSynapseIdx < pNeuron->getOutputSynapseCount(); ++iSynapseIdx) {
                const Synapse* pSynapse = pNeuron->getOutputSynapse(iSynapseIdx);
                if (pSynapse && pSynapse->isAxon()) {
                    if (!pSynapse->isValid()) {
                        return false; // Synapse validation will check weight == 1.0
                    }
                }
            }
        }
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
vector<double> Network::predict(const vector<double>& inputs) {
    if (!isValid()) {
        throw runtime_error("Network is not valid for inference");
    }
    
    if (m_layers.empty()) {
        throw runtime_error("Network has no layers");
    }
    
    // Check input size matches first layer
    if (static_cast<int>(inputs.size()) != m_layers[0]->getNeuronCount()) {
        throw runtime_error("Input size mismatch with first layer neuron count");
    }
    
    // Reset computation state
    resetComputationState();
    
    // Forward propagate through all layers
    vector<double> currentOutputs = inputs;
    
    for (size_t uLayerIndex = 0; uLayerIndex < m_layers.size(); ++uLayerIndex) {
        Layer* pLayer = m_layers[uLayerIndex].get();
        
        if (uLayerIndex == 0) {
            // For first layer, each neuron gets one input value
            // Check if neurons have input synapses
            vector<vector<double>> layerInputs;
            for (int iNeuronIdx = 0; iNeuronIdx < pLayer->getNeuronCount(); ++iNeuronIdx) {
                const Neuron* pNeuron = pLayer->getNeuron(iNeuronIdx);
                if (pNeuron && pNeuron->getInputSynapseCount() > 0) {
                    // Neuron expects inputs through synapses
                    layerInputs.push_back({inputs[iNeuronIdx]});
                } else {
                    // Neuron can accept direct input
                    layerInputs.push_back({inputs[iNeuronIdx]});
                }
            }
            currentOutputs = pLayer->forwardPropagate(layerInputs);
        } else {
            // For subsequent layers, neurons get outputs from previous layer
            vector<vector<double>> layerInputs;
            for (int iNeuronIndex = 0; iNeuronIndex < pLayer->getNeuronCount(); ++iNeuronIndex) {
                const Neuron* pNeuron = pLayer->getNeuron(iNeuronIndex);
                if (pNeuron && pNeuron->getInputSynapseCount() > 0) {
                    // Neuron has input synapses, provide all previous outputs
                    // This assumes a fully connected architecture or that the neuron
                    // will only use as many inputs as it has synapses
                    vector<double> neuronInputs;
                    for (int iSynapseIdx = 0; iSynapseIdx < pNeuron->getInputSynapseCount(); ++iSynapseIdx) {
                        if (iSynapseIdx < static_cast<int>(currentOutputs.size())) {
                            neuronInputs.push_back(currentOutputs[iSynapseIdx]);
                        } else {
                            neuronInputs.push_back(0.0); // Default value if not enough outputs
                        }
                    }
                    layerInputs.push_back(neuronInputs);
                } else {
                    // Neuron without synapses, provide empty input vector
                    layerInputs.push_back(vector<double>());
                }
            }
            currentOutputs = pLayer->forwardPropagate(layerInputs);
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
    (void)defaultWeight; // Suppress unused parameter warning
    if (m_layers.size() < 2) {
        return true; // Nothing to connect
    }
    
    for (size_t uLayerIdx = 0; uLayerIdx < m_layers.size() - 1; ++uLayerIdx) {
        if (!m_layers[uLayerIdx]->connectToLayer(*m_layers[uLayerIdx + 1])) {
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
string Network::getStructureInfo() const {
    ostringstream oss;
    oss << "Network: " << m_name << "\n";
    oss << "Layers: " << getLayerCount() << "\n";
    oss << "Total Neurons: " << getNeuronCount() << "\n";
    oss << "Total Synapses (含输入输出): " << getSynapseCount() << "\n";
    oss << "Valid: " << (isValid() ? "Yes" : "No") << "\n";
    
    for (int iLayerIdx = 0; iLayerIdx < getLayerCount(); ++iLayerIdx) {
        const Layer* pLayer = getLayer(iLayerIdx);
        oss << "  Layer " << iLayerIdx << ": " << pLayer->getNeuronCount() << " neurons\n";
    }
    
    return oss.str();                                                                                                                                                                       
}

//-------------------------------------------------------------
//【函数名称】setImportError
//【函数功能】设置导入错误信息（用于缓存验证错误）
//【参数】errorMessage：错误信息
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::setImportError(const string& errorMessage) {
    m_hasImportErrors = true;
    m_importErrorMessage = errorMessage;
    m_validationCacheValid = true;
}

//-------------------------------------------------------------
//【函数名称】clearImportErrors
//【函数功能】清除导入错误信息（网络修改后调用）
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::clearImportErrors() {
    m_hasImportErrors = false;
    m_importErrorMessage = "";
    m_validationCacheValid = false;
}

//-------------------------------------------------------------
//【函数名称】hasImportErrors
//【函数功能】检查是否有导入错误
//【参数】无
//【返回值】bool，是否有导入错误
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::hasImportErrors() const {
    return m_hasImportErrors;
}

//-------------------------------------------------------------
//【函数名称】getImportErrorMessage
//【函数功能】获取导入错误信息
//【参数】无
//【返回值】string，错误信息
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string Network::getImportErrorMessage() const {
    return m_importErrorMessage;
}

//-------------------------------------------------------------
//【函数名称】invalidateValidationCache
//【函数功能】使验证缓存失效（网络修改后调用）
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void Network::invalidateValidationCache() {
    m_validationCacheValid = false;
    // Don't clear import errors here - they remain until explicitly cleared
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
    if (m_layers.empty()) {
        return false;
    }
    
    // Create a map of all neurons with their states for DFS cycle detection
    // 0 = unvisited, 1 = visiting (in current path), 2 = visited (completed)
    map<const Neuron*, int> neuronStates;
    
    // Initialize all neurons as unvisited
    for (const auto& layer : m_layers) {
        for (int iNeuronIdx = 0; iNeuronIdx < layer->getNeuronCount(); ++iNeuronIdx) {
            const Neuron* pNeuron = layer->getNeuron(iNeuronIdx);
            if (pNeuron) {
                neuronStates[pNeuron] = 0;
            }
        }
    }
    
    // DFS function to detect cycles
    function<bool(const Neuron*)> dfsHasCycle = [&](const Neuron* pNeuron) -> bool {
        if (neuronStates[pNeuron] == 1) {
            // Found a back edge - cycle detected
            return true;
        }
        
        if (neuronStates[pNeuron] == 2) {
            // Already processed this neuron
            return false;
        }
        
        // Mark as visiting
        neuronStates[pNeuron] = 1;
        
        // Check all outgoing connections
        for (int iSynapseIdx = 0; iSynapseIdx < pNeuron->getOutputSynapseCount(); ++iSynapseIdx) {
            const Synapse* pSynapse = pNeuron->getOutputSynapse(iSynapseIdx);
            if (pSynapse && pSynapse->getTargetNeuron()) {
                if (dfsHasCycle(pSynapse->getTargetNeuron())) {
                    return true;
                }
            }
        }
        
        // Mark as completely visited
        neuronStates[pNeuron] = 2;
        return false;
    };
    
    // Check each unvisited neuron
    for (const auto& layer : m_layers) {
        for (int iNeuronIdx = 0; iNeuronIdx < layer->getNeuronCount(); ++iNeuronIdx) {
            const Neuron* pNeuron = layer->getNeuron(iNeuronIdx);
            if (pNeuron && neuronStates[pNeuron] == 0) {
                if (dfsHasCycle(pNeuron)) {
                    return true;
                }
            }
        }
    }
    
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
    if (m_layers.empty()) {
        return false;
    }
    
    // Check that each layer has neurons
    for (const auto& layer : m_layers) {
        if (layer->getNeuronCount() == 0) {
            return false;
        }
    }

    // Check input layer (first layer)
    const Layer* pInputLayer = m_layers[0].get();
    for (int iNeuronIdx = 0; iNeuronIdx < pInputLayer->getNeuronCount(); ++iNeuronIdx) {
        const Neuron* pNeuron = pInputLayer->getNeuron(iNeuronIdx);
        if (!pNeuron) {
            return false;
        }
        // Input neurons should have some way to receive input
        // (either external input capability or input synapses)
    }
    
    // Check output layer (last layer)
    const Layer* pOutputLayer = m_layers.back().get();
    for (int iNeuronIdx = 0; iNeuronIdx < pOutputLayer->getNeuronCount(); ++iNeuronIdx) {
        const Neuron* pNeuron = pOutputLayer->getNeuron(iNeuronIdx);
        if (!pNeuron) {
            return false;
        }
        // Output neurons should have some way to provide output
        // (either external output capability or output synapses)
    }
    
    // Check hidden layers (all layers except first and last if there are more than 2 layers)
    if (m_layers.size() > 2) {
        for (size_t uLayerIdx = 1; uLayerIdx < m_layers.size() - 1; ++uLayerIdx) {
            const Layer* pLayer = m_layers[uLayerIdx].get();
            for (int iNeuronIdx = 0; iNeuronIdx < pLayer->getNeuronCount(); ++iNeuronIdx) {
                const Neuron* pNeuron = pLayer->getNeuron(iNeuronIdx);
                if (!pNeuron) {
                    return false;
                }
                // Hidden neurons should have both input and output connections
                if (pNeuron->getInputSynapseCount() == 0 && pNeuron->getOutputSynapseCount() == 0) {
                    // Isolated neuron - doesn't participate in data flow
                    return false;
                }
            }
        }
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】removeNeuron
//【函数功能】安全移除指定层中的神经元（清理所有相关连接）
//【参数】layerIndex：层索引，neuronIndex：神经元索引
//【返回值】bool，是否移除成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool Network::removeNeuron(int layerIndex, int neuronIndex) {
    if (layerIndex < 0 || layerIndex >= static_cast<int>(m_layers.size())) {
        return false;
    }
    
    Layer* targetLayer = m_layers[layerIndex].get();
    if (!targetLayer || neuronIndex < 0 || neuronIndex >= targetLayer->getNeuronCount()) {
        return false;
    }
    
    Neuron* neuronToDelete = targetLayer->getNeuron(neuronIndex);
    if (!neuronToDelete) {
        return false;
    }
    
    // Remove all synapses that reference the neuron to be deleted
    for (auto& layer : m_layers) {
        for (int i = 0; i < layer->getNeuronCount(); ++i) {
            Neuron* neuron = layer->getNeuron(i);
            if (neuron && neuron != neuronToDelete) {
                // Remove synapses targeting the deleted neuron
                for (int j = neuron->getOutputSynapseCount() - 1; j >= 0; --j) {
                    const Synapse* synapse = neuron->getOutputSynapse(j);
                    if (synapse && synapse->getTargetNeuron() == neuronToDelete) {
                        neuron->removeOutputSynapse(j);
                    }
                }
                // Remove synapses from the deleted neuron
                for (int j = neuron->getInputSynapseCount() - 1; j >= 0; --j) {
                    const Synapse* synapse = neuron->getInputSynapse(j);
                    if (synapse && synapse->getSourceNeuron() == neuronToDelete) {
                        neuron->removeInputSynapse(j);
                    }
                }
            }
        }
    }
    
    // Remove the neuron from its layer
    return targetLayer->removeNeuron(neuronIndex);
}
