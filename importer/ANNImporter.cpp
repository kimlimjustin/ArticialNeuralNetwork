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
    skipWhitespaceAndComments(file);
    
    // Read network name (optional)
    std::string token = readToken(file);
    if (token == "NETWORK") {
        std::string networkName = readToken(file);
        network.setName(networkName);
    } else {
        // Put token back by seeking back
        file.seekg(static_cast<std::streamoff>(-static_cast<std::streamoff>(token.length())), std::ios::cur);
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
    skipWhitespaceAndComments(file);
    
    std::string token = readToken(file);
    if (token != "LAYERS") {
        return false;
    }
    
    int layerCount;
    file >> layerCount;
    if (file.fail() || layerCount <= 0) {
        return false;
    }
    
    // Parse each layer
    for (int i = 0; i < layerCount; ++i) {
        skipWhitespaceAndComments(file);
        
        token = readToken(file);
        if (token != "LAYER") {
            return false;
        }
        
        int neuronCount;
        file >> neuronCount;
        if (file.fail() || neuronCount <= 0) {
            return false;
        }
        
        // Create layer
        auto layer = std::unique_ptr<Layer>(new Layer());
        
        // Parse neurons for this layer
        if (!parseNeuronInformation(file, *layer, neuronCount)) {
            return false;
        }
        
        network.addLayer(std::move(layer));
    }
    
    return true;
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
    skipWhitespaceAndComments(file);
    
    std::string token = readToken(file);
    if (token != "CONNECTIONS") {
        return true; // Connections section is optional
    }
    
    int connectionCount;
    file >> connectionCount;
    if (file.fail() || connectionCount < 0) {
        return false;
    }
    
    // Parse each connection
    for (int i = 0; i < connectionCount; ++i) {
        skipWhitespaceAndComments(file);
        
        token = readToken(file);
        if (token != "CONNECTION") {
            return false;
        }
        
        int fromLayer, fromNeuron, toLayer, toNeuron;
        double weight;
        
        file >> fromLayer >> fromNeuron >> toLayer >> toNeuron >> weight;
        if (file.fail()) {
            return false;
        }
        
        // Create connection
        Layer* sourceLayer = network.getLayer(fromLayer);
        Layer* targetLayer = network.getLayer(toLayer);
        
        if (!sourceLayer || !targetLayer) {
            return false;
        }
        
        Neuron* sourceNeuron = sourceLayer->getNeuron(fromNeuron);
        Neuron* targetNeuron = targetLayer->getNeuron(toNeuron);
        
        if (!sourceNeuron || !targetNeuron) {
            return false;
        }
        
        if (!sourceNeuron->connectTo(*targetNeuron, weight)) {
            return false;
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
