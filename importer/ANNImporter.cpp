//-------------------------------------------------------------
//【文件名】ANNImporter.cpp
//【功能模块和目的】ANN格式神经网络模型导入器实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "ANNImporter.h"
#include "../model/ActivationFunction.h"
#include "../utils/FileUtils.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

//-------------------------------------------------------------
//【函数名称】ANNImporter
//【函数功能】构造函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
ANNImporter::ANNImporter() {
}

//-------------------------------------------------------------
//【函数名称】~ANNImporter
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
ANNImporter::~ANNImporter() {
}

//-------------------------------------------------------------
//【函数名称】importNetwork
//【函数功能】导入神经网络
//【参数】filename：文件名
//【返回值】std::unique_ptr<Network>，导入的网络指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::unique_ptr<Network> ANNImporter::importNetwork(const std::string& filename) {
    if (!isFormatSupported(filename)) {
        return nullptr;
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }
    
    auto network = std::unique_ptr<Network>(new Network());
    
    try {
        // Parse network header
        if (!parseNetworkHeader(file, *network)) {
            return nullptr;
        }
        
        // Parse layer information
        if (!parseLayerInformation(file, *network)) {
            return nullptr;
        }
        
        // Parse connections
        if (!parseConnections(file, *network)) {
            return nullptr;
        }
        
        // Validate imported network
        if (!validateImportedNetwork(network.get())) {
            return nullptr;
        }
        
        return network;
    }
    catch (const std::exception&) {
        return nullptr;
    }
}

//-------------------------------------------------------------
//【函数名称】getSupportedExtensions
//【函数功能】获取支持的文件扩展名
//【参数】无
//【返回值】支持的文件扩展名字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string ANNImporter::getSupportedExtensions() const {
    return ".ann";
}

//-------------------------------------------------------------
//【函数名称】getImporterName
//【函数功能】获取导入器名称
//【参数】无
//【返回值】导入器名称字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string ANNImporter::getImporterName() const {
    return "ANN Importer";
}

//-------------------------------------------------------------
//【函数名称】parseNetworkHeader
//【函数功能】解析网络头部信息
//【参数】file：文件流，network：网络对象
//【返回值】解析成功返回true，失败返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool ANNImporter::parseNetworkHeader(std::ifstream& file, Network& network) {
    std::string line;
    
    // Read lines until we find the network name or end of file
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Check for network name starting with 'G'
        if (line[0] == 'G') {
            std::istringstream iss(line);
            std::string prefix, networkName;
            if (iss >> prefix >> networkName) {
                network.setName(networkName);
            }
            return true;
        }
        
        // If we encounter other data, seek back and return
        file.seekg(static_cast<std::streamoff>(-static_cast<std::streamoff>(line.length() + 1)), std::ios::cur);
        return true;
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】parseLayerInformation
//【函数功能】解析层信息
//【参数】file：文件流，network：网络对象
//【返回值】解析成功返回true，失败返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool ANNImporter::parseLayerInformation(std::ifstream& file, Network& network) {
    std::string line;
    std::vector<std::pair<double, int>> neurons; // bias, activation function type
    std::vector<std::pair<int, int>> layers; // start neuron, end neuron
    
    // Reset file position to beginning
    file.clear();
    file.seekg(0, std::ios::beg);
    
    // First pass: collect neuron and layer information
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        char prefix;
        iss >> prefix;
        
        if (prefix == 'N') {
            // Neuron definition: N bias activation_type
            double bias;
            int activationType;
            if (iss >> bias >> activationType) {
                neurons.push_back({bias, activationType});
            }
        }
        else if (prefix == 'L') {
            // Layer definition: L start_neuron end_neuron
            int startNeuron, endNeuron;
            if (iss >> startNeuron >> endNeuron) {
                layers.push_back({startNeuron, endNeuron});
            }
        }
    }
    
    // Create layers and neurons
    for (const auto& layerInfo : layers) {
        auto layer = std::make_unique<Layer>();
        
        for (int i = layerInfo.first; i <= layerInfo.second; ++i) {
            if (i >= 0 && i < static_cast<int>(neurons.size())) {
                auto neuron = std::make_unique<Neuron>();
                neuron->setBias(neurons[i].first);
                
                // Set activation function based on type
                if (neurons[i].second == 0) {
                    // Type 0 = Linear (no activation)
                    neuron->setActivationFunction(createActivationFunction("Linear"));
                } else {
                    // Default to linear for unknown types
                    neuron->setActivationFunction(createActivationFunction("Linear"));
                }
                
                layer->addNeuron(std::move(neuron));
            }
        }
        
        network.addLayer(std::move(layer));
    }
    
    return !layers.empty();
}

//-------------------------------------------------------------
//【函数名称】parseNeuronInformation
//【函数功能】解析神经元信息
//【参数】file：文件流，layer：层对象，neuronCount：神经元数量
//【返回值】解析成功返回true，失败返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool ANNImporter::parseNeuronInformation(std::ifstream& file, Layer& layer, int neuronCount) {
    for (int i = 0; i < neuronCount; ++i) {
        skipWhitespaceAndComments(file);
        
        std::string token = readToken(file);
        if (token != "NEURON") {
            return false;
        }
        
        // Read bias
        double bias;
        file >> bias;
        if (file.fail()) {
            return false;
        }
        
        // Read activation function (optional)
        std::unique_ptr<ActivationFunction> activationFunc = nullptr;
        token = readToken(file);
        if (token == "ACTIVATION") {
            std::string funcName = readToken(file);
            activationFunc = parseActivationFunction(funcName);
        } else {
            // Put token back
            file.seekg(static_cast<std::streamoff>(-static_cast<std::streamoff>(token.length())), std::ios::cur);
        }
        
        // Create neuron
        auto neuron = std::unique_ptr<Neuron>(new Neuron(bias, std::move(activationFunc)));
        layer.addNeuron(std::move(neuron));
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】parseConnections
//【函数功能】解析连接信息
//【参数】file：文件流，network：网络对象
//【返回值】解析成功返回true，失败返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool ANNImporter::parseConnections(std::ifstream& file, Network& network) {
    std::string line;
    
    // Reset file position to beginning  
    file.clear();
    file.seekg(0, std::ios::beg);
    
    // Parse synapse connections with format: S from_neuron to_neuron weight
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        char prefix;
        iss >> prefix;
        
        if (prefix == 'S') {
            int fromNeuron, toNeuron;
            double weight;
            
            if (iss >> fromNeuron >> toNeuron >> weight) {
                // Handle external input connections (from -1)
                if (fromNeuron == -1 && toNeuron >= 0) {
                    // Create input synapse for the target neuron
                    Neuron* targetNeuron = findNeuronByGlobalIndex(network, toNeuron);
                    if (targetNeuron) {
                        auto synapse = std::make_unique<Synapse>(weight, nullptr, targetNeuron, false);
                        targetNeuron->addInputSynapse(std::move(synapse));
                    }
                    continue;
                }
                
                // Handle external output connections (to -1)
                if (toNeuron == -1 && fromNeuron >= 0) {
                    // These represent outputs - we can ignore them for now
                    // as they don't affect the network structure
                    continue;
                }
                
                // Handle inter-neuron connections
                if (fromNeuron >= 0 && toNeuron >= 0) {
                    Neuron* sourceNeuron = findNeuronByGlobalIndex(network, fromNeuron);
                    Neuron* targetNeuron = findNeuronByGlobalIndex(network, toNeuron);
                    
                    if (sourceNeuron && targetNeuron) {
                        sourceNeuron->connectTo(*targetNeuron, weight);
                    }
                }
            }
        }
    }
    
    return true;
}

//-------------------------------------------------------------
//【函数名称】skipWhitespaceAndComments
//【函数功能】跳过空白和注释
//【参数】file：文件流
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ANNImporter::skipWhitespaceAndComments(std::ifstream& file) {
    char ch;
    while (file.get(ch)) {
        if (std::isspace(ch)) {
            continue; // Skip whitespace
        }
        else if (ch == '#') {
            // Skip comment line
            std::string line;
            std::getline(file, line);
        }
        else {
            // Put character back
            file.putback(ch);
            break;
        }
    }
}

//-------------------------------------------------------------
//【函数名称】readToken
//【函数功能】读取一个标记
//【参数】file：文件流
//【返回值】标记字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string ANNImporter::readToken(std::ifstream& file) {
    skipWhitespaceAndComments(file);
    
    std::string token;
    char ch;
    
    while (file.get(ch)) {
        if (std::isspace(ch)) {
            break;
        }
        token += ch;
    }
    
    return token;
}

//-------------------------------------------------------------
//【函数名称】parseActivationFunction
//【函数功能】解析激活函数
//【参数】functionName：函数名称
//【返回值】激活函数指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::unique_ptr<ActivationFunction> ANNImporter::parseActivationFunction(const std::string& functionName) {
    return createActivationFunction(functionName);
}

//-------------------------------------------------------------
//【函数名称】findNeuronByGlobalIndex
//【函数功能】根据全局索引查找神经元
//【参数】network：网络引用，globalIndex：全局索引
//【返回值】Neuron*，找到的神经元指针，未找到返回nullptr
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
Neuron* ANNImporter::findNeuronByGlobalIndex(Network& network, int globalIndex) {
    int currentIndex = 0;
    
    for (int layerIdx = 0; layerIdx < network.getLayerCount(); ++layerIdx) {
        Layer* layer = network.getLayer(layerIdx);
        if (!layer) continue;
        
        for (int neuronIdx = 0; neuronIdx < layer->getNeuronCount(); ++neuronIdx) {
            if (currentIndex == globalIndex) {
                return layer->getNeuron(neuronIdx);
            }
            currentIndex++;
        }
    }
    
    return nullptr;
}
