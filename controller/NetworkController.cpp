//-------------------------------------------------------------
//【文件名】NetworkController.cpp
//【功能模块和目的】神经网络控制器（单例）实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "NetworkController.hpp"
#include "../importer/ANNImporter.hpp"
#include "../exporter/ANNExporter.hpp"
#include <stdexcept>
#include <sstream>

using namespace std;

unique_ptr<NetworkController> NetworkController::m_instance = nullptr;

//-------------------------------------------------------------
//【函数名称】NetworkController
//【函数功能】构造函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
NetworkController::NetworkController() : m_network(nullptr) {
}

//-------------------------------------------------------------
//【函数名称】~NetworkController
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
NetworkController::~NetworkController() = default;

//-------------------------------------------------------------
//【函数名称】getInstance
//【函数功能】获取单例实例
//【参数】无
//【返回值】NetworkController&，单例引用
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
NetworkController& NetworkController::getInstance() {
    if (m_instance == nullptr) {
        m_instance = unique_ptr<NetworkController>(new NetworkController());
    }
    return *m_instance;
}

//-------------------------------------------------------------
//【函数名称】cleanup
//【函数功能】清理单例实例
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void NetworkController::cleanup() {
    m_instance.reset();
}

//-------------------------------------------------------------
//【函数名称】importNetwork
//【函数功能】导入神经网络3

//【参数】filename：文件名
//【返回值】bool，是否导入成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::importNetwork(const string& filename) {
    try {
        ANNImporter importer;
        m_network = importer.importNetwork(filename);
        return m_network != nullptr;
    }
    catch (const exception&) {
        m_network = nullptr;
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】exportNetwork
//【函数功能】导出神经网络
//【参数】filename：文件名
//【返回值】bool，是否导出成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::exportNetwork(const string& filename) const {
    if (!hasNetwork()) {
        return false;
    }
    
    try {
        ANNExporter exporter;
        return exporter.exportNetwork(*m_network, filename);
    }
    catch (const exception&) {
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】hasNetwork
//【函数功能】检查是否存在神经网络
//【参数】无
//【返回值】bool，是否存在神经网络
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::hasNetwork() const {
    return m_network != nullptr;
}

//-------------------------------------------------------------
//【函数名称】validateNetwork
//【函数功能】验证当前网络结构
//【参数】无
//【返回值】bool，网络有效返回true，否则返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::validateNetwork() const {
    if (!hasNetwork()) {
        return false;
    }
    
    return m_network->isValid();
}

//-------------------------------------------------------------
//【函数名称】getNetworkStatistics
//【函数功能】获取神经网络统计信息
//【参数】无
//【返回值】字符串，统计信息
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string NetworkController::getNetworkStatistics() const {
    if (!hasNetwork()) {
        return "No network loaded.";
    }
    
    ostringstream oss;
    oss << "Network Statistics:\n";
    oss << "  Total Layers: " << m_network->getLayerCount() << "\n";
    oss << "  Total Neurons: " << m_network->getNeuronCount() << "\n";
    oss << "  Total Synapses (含输入输出): " << m_network->getSynapseCount() << "\n";
    oss << "  Valid: " << (m_network->isValid() ? "Yes" : "No");
    
    // Include import error information if any
    if (m_network->hasImportErrors()) {
        oss << "\n\n[IMPORT WARNINGS]\n";
        oss << m_network->getImportErrorMessage();
    }
    
    return oss.str();
}

//-------------------------------------------------------------
//【函数名称】getLayerInformation
//【函数功能】获取神经网络层信息
//【参数】无
//【返回值】字符串，层信息
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string NetworkController::getLayerInformation() const {
    if (!hasNetwork()) {
        return "No network loaded.";
    }
    
    ostringstream oss;
    oss << "Layer Information:\n";
    
    for (int i = 0; i < m_network->getLayerCount(); ++i) {
        const Layer* layer = m_network->getLayer(i);
        if (layer) {
            oss << "  Layer " << i << ": " << layer->getNeuronCount() << " neurons\n";
        }
    }
    
    return oss.str();
}

//-------------------------------------------------------------
//【函数名称】getNeuronInformation
//【函数功能】获取指定层的神经元信息
//【参数】layerIndex：层索引
//【返回值】字符串，神经元信息
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string NetworkController::getNeuronInformation(int layerIndex) const {
    if (!hasNetwork()) {
        return "No network loaded.";
    }
    
    const Layer* layer = m_network->getLayer(layerIndex);
    if (!layer) {
        return "Invalid layer index.";
    }
    
    ostringstream oss;
    oss << "Neurons in Layer " << layerIndex << ":\n";
    
    for (int i = 0; i < layer->getNeuronCount(); ++i) {
        const Neuron* neuron = layer->getNeuron(i);
        if (neuron) {
            oss << "  Neuron " << i << ": bias = " << neuron->getBias() << "\n";
        }
    }
    
    return oss.str();
}

//-------------------------------------------------------------
//【函数名称】getNeuronConnections
//【函数功能】获取神经元连接信息
//【参数】layerIndex：层索引，neuronIndex：神经元索引
//【返回值】字符串，连接信息
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string NetworkController::getNeuronConnections(int layerIndex, int neuronIndex) const {
    if (!hasNetwork()) {
        return "No network loaded.";
    }
    
    const Layer* layer = m_network->getLayer(layerIndex);
    if (!layer) {
        return "Invalid layer index.";
    }
    
    const Neuron* neuron = layer->getNeuron(neuronIndex);
    if (!neuron) {
        return "Invalid neuron index.";
    }
    
    ostringstream oss;
    oss << "Connections for Neuron " << neuronIndex << " in Layer " << layerIndex << ":\n";
    oss << "  Input connections (dendrites): " << neuron->getInputSynapseCount() << "\n";
    oss << "  Output connections (axon): " << neuron->getOutputSynapseCount() << "\n";
    
    // Additional connection details could be added here
    
    return oss.str();
}

//-------------------------------------------------------------
//【函数名称】addLayer
//【函数功能】添加一层神经元
//【参数】无
//【返回值】bool，是否添加成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::addLayer() {
    if (!hasNetwork()) {
        m_network = unique_ptr<Network>(new Network());
    }
    
    try {
        m_network->addLayer(unique_ptr<Layer>(new Layer()));
        // Clear import errors when network is modified
        m_network->clearImportErrors();
        return true;
    }
    catch (const exception&) {
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】deleteLayer
//【函数功能】删除一层神经元
//【参数】layerIndex：层索引
//【返回值】bool，是否删除成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::deleteLayer(int layerIndex) {
    if (!hasNetwork()) {
        return false;
    }
    
    try {
        bool result = m_network->removeLayer(layerIndex);
        if (result) {
            // Clear import errors when network is modified
            m_network->clearImportErrors();
        }
        return result;
    }
    catch (const exception&) {
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】modifyNeuronBias
//【函数功能】修改神经元的偏置值
//【参数】layerIndex：层索引，neuronIndex：神经元索引，bias：新的偏置值
//【返回值】bool，是否修改成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::modifyNeuronBias(int layerIndex, int neuronIndex, double bias) {
    if (!hasNetwork()) {
        return false;
    }
    
    Layer* layer = m_network->getLayer(layerIndex);
    if (!layer) {
        return false;
    }
    
    Neuron* neuron = layer->getNeuron(neuronIndex);
    if (!neuron) {
        return false;
    }
    
    try {
        neuron->setBias(bias);
        // Clear import errors when network is modified
        m_network->clearImportErrors();
        return true;
    }
    catch (const exception&) {
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】deleteNeuron
//【函数功能】删除指定的神经元
//【参数】layerIndex：层索引，neuronIndex：神经元索引
//【返回值】bool，是否删除成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::deleteNeuron(int layerIndex, int neuronIndex) {
    if (!hasNetwork()) {
        return false;
    }
    
    Layer* layer = m_network->getLayer(layerIndex);
    if (!layer) {
        return false;
    }
    
    try {
        return layer->removeNeuron(neuronIndex);
    }
    catch (const exception&) {
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】connectNeurons
//【函数功能】连接两个神经元
//【参数】fromLayer：源层索引，fromNeuron：源神经元索引，toLayer：目标层索引，toNeuron：目标神经元索引，weight：连接权重
//【返回值】bool，是否连接成功
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool NetworkController::connectNeurons(int fromLayer, int fromNeuron, int toLayer, int toNeuron, double weight) {
    if (!hasNetwork()) {
        return false;
    }
    
    // Check if layers are adjacent
    if (toLayer != fromLayer + 1) {
        return false;
    }
    
    Layer* sourceLayer = m_network->getLayer(fromLayer);
    Layer* targetLayer = m_network->getLayer(toLayer);
    
    if (!sourceLayer || !targetLayer) {
        return false;
    }
    
    Neuron* sourceNeuron = sourceLayer->getNeuron(fromNeuron);
    Neuron* targetNeuron = targetLayer->getNeuron(toNeuron);
    
    if (!sourceNeuron || !targetNeuron) {
        return false;
    }
    
    try {
        return sourceNeuron->connectTo(*targetNeuron, weight);
    }
    catch (const exception&) {
        return false;
    }
}

//-------------------------------------------------------------
//【函数名称】getInputSize
//【函数功能】获取输入层大小
//【参数】无
//【返回值】输入大小，失败返回-1
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int NetworkController::getInputSize() const {
    if (!hasNetwork()) {
        return -1;
    }
    
    const Layer* firstLayer = m_network->getLayer(0);
    if (!firstLayer) {
        return -1;
    }
    
    return firstLayer->getNeuronCount();
}

//-------------------------------------------------------------
//【函数名称】runInference
//【函数功能】运行推理
//【参数】inputs：输入数据
//【返回值】推理结果
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
vector<double> NetworkController::runInference(const vector<double>& inputs) const {
    if (!hasNetwork()) {
        throw runtime_error("No network loaded");
    }
    
    if (!m_network->isValid()) {
        throw runtime_error("Network is not valid");
    }
    
    return m_network->predict(inputs);
}

//-------------------------------------------------------------
//【函数名称】getValidationDetails
//【函数功能】获取详细的验证信息，包括导入错误
//【参数】无
//【返回值】string，包含详细验证信息的字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string NetworkController::getValidationDetails() const {
    if (!hasNetwork()) {
        return "No network loaded.";
    }
    
    ostringstream oss;
    
    // Basic validation result
    bool isValid = m_network->isValid();
    oss << "Network Validation: " << (isValid ? "PASSED" : "FAILED") << "\n";
    
    // If there are import errors, show them
    if (m_network->hasImportErrors()) {
        oss << "\nIMPORT VALIDATION ERRORS:\n";
        oss << m_network->getImportErrorMessage() << "\n";
        oss << "\nNote: The network has been auto-corrected during import, but the original file contains specification violations.\n";
        oss << "Please review and correct the source file to eliminate these warnings.\n";
    } else if (!isValid) {
        oss << "\nNetwork structure is invalid. Please check:\n";
        oss << "- All layers have at least one neuron\n";
        oss << "- Network has at least one layer\n";
        oss << "- All axon weights are 1.0 (per specification)\n";
        oss << "- Network structure is consistent\n";
    } else {
        oss << "\nNetwork structure is valid and ready for inference.";
    }
    
    return oss.str();
}
