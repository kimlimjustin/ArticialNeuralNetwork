//-------------------------------------------------------------
//【文件名】ANNImporter.cpp
//【功能模块和目的】ANN格式神经网络模型导入器实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "ANNImporter.hpp"
#include "../model/neural_components/Network.hpp"
#include "../model/neural_components/Layer.hpp"
#include "../model/neural_components/Neuron.hpp"
#include "../model/neural_components/Synapse.hpp"
#include "../model/activation_functions/LinearFunction.hpp"
#include "../model/activation_functions/SigmoidFunction.hpp"
#include "../model/activation_functions/ReLUFunction.hpp"
#include "../model/activation_functions/TanhFunction.hpp"
#include "../model/activation_functions/ActivationFunction.hpp"
#include "../utils/FileUtils.hpp"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace std;

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
ANNImporter::~ANNImporter() = default;

//-------------------------------------------------------------
//【函数名称】importNetwork
//【函数功能】导入神经网络
//【参数】filename：文件名
//【返回值】unique_ptr<Network>，导入的网络指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
unique_ptr<Network> ANNImporter::importNetwork(const string& filename) {
    if (!isFormatSupported(filename)) {
        return nullptr;
    }
    
    ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }
    
    auto network = unique_ptr<Network>(new Network());
    
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
    catch (const exception&) {
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
string ANNImporter::getSupportedExtensions() const {
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
string ANNImporter::getImporterName() const {
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
bool ANNImporter::parseNetworkHeader(ifstream& file, Network& network) {
    string line;
    
    // Read lines until we find the network name or end of file
    while (getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Check for network name starting with 'G'
        if (line[0] == 'G') {
            istringstream iss(line);
            string prefix;
            string networkName;
            if (iss >> prefix >> networkName) {
                network.setName(networkName);
            }
            return true;
        }
        
        // If we encounter other data, seek back and return
        file.seekg(static_cast<streamoff>(-static_cast<streamoff>(line.length() + 1)), ios::cur);
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
bool ANNImporter::parseLayerInformation(ifstream& file, Network& network) {
    string line;
    vector<pair<double, int>> neurons; // bias, activation function type
    vector<pair<int, int>> layers; // start neuron, end neuron
    
    // Reset file position to beginning
    file.clear();
    file.seekg(0, ios::beg);
    
    // First pass: collect neuron and layer information
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        istringstream iss(line);
        char cPrefix;
        iss >> cPrefix;
        
        if (cPrefix == 'N') {
            // Neuron definition: N bias activation_type
            double rBias;
            int iActivationType;
            if (iss >> rBias >> iActivationType) {
                neurons.push_back({rBias, iActivationType});
            }
        }
        else if (cPrefix == 'L') {
            // Layer definition: L start_neuron end_neuron
            int iStartNeuron;
            int iEndNeuron;
            if (iss >> iStartNeuron >> iEndNeuron) {
                layers.push_back({iStartNeuron, iEndNeuron});
            }
        }
    }
    
    // Create layers and neurons
    for (const auto& layerInfo : layers) {
        unique_ptr<Layer> layer(new Layer());
        
        for (int iNeuronIdx = layerInfo.first; iNeuronIdx <= layerInfo.second; ++iNeuronIdx) {
            if (iNeuronIdx >= 0 && iNeuronIdx < static_cast<int>(neurons.size())) {
                auto neuron = createNeuronWithActivation(neurons[iNeuronIdx].first, neurons[iNeuronIdx].second);
                layer->addNeuron(move(neuron));
            }
        }
        
        network.addLayer(move(layer));
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
bool ANNImporter::parseNeuronInformation(ifstream& file, Layer& layer, int neuronCount) {
    for (int iNeuronIdx = 0; iNeuronIdx < neuronCount; ++iNeuronIdx) {
        skipWhitespaceAndComments(file);
        
        string token = readToken(file);
        if (token != "NEURON") {
            return false;
        }
        
        // Read bias
        double rBias;
        file >> rBias;
        if (file.fail()) {
            return false;
        }
        
        // Read activation function (optional)
        unique_ptr<ActivationFunction> activationFunc = nullptr;
        token = readToken(file);
        if (token == "ACTIVATION") {
            string funcName = readToken(file);
            activationFunc = parseActivationFunction(funcName);
        } else {
            // Put token back
            file.seekg(static_cast<streamoff>(-static_cast<streamoff>(token.length())), ios::cur);
        }
        
        // Create neuron
        auto neuron = unique_ptr<Neuron>(new Neuron(rBias, move(activationFunc)));
        layer.addNeuron(move(neuron));
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
bool ANNImporter::parseConnections(ifstream& file, Network& network) {
    string line;
    vector<string> invalidAxonConnections; // Track invalid axon weights
    
    // Reset file position to beginning  
    file.clear();
    file.seekg(0, ios::beg);
    
    // Parse synapse connections with format: S from_neuron to_neuron weight
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        istringstream iss(line);
        char cPrefix;
        iss >> cPrefix;
        
        if (cPrefix == 'S') {
            int iFromNeuron;
            int iToNeuron;
            double rWeight;
            
            if (iss >> iFromNeuron >> iToNeuron >> rWeight) {
                // Handle external input connections (from -1)
                if (iFromNeuron == -1 && iToNeuron >= 0) {
                    // This is a dendrite (input synapse) - weight can be any value
                    Neuron* pTargetNeuron = findNeuronByGlobalIndex(network, iToNeuron);
                    if (pTargetNeuron) {
                        unique_ptr<Synapse> synapse(new Synapse(rWeight, nullptr, pTargetNeuron, false));
                        pTargetNeuron->addInputSynapse(move(synapse));
                    }
                    continue;
                }
                
                // Handle external output connections (to -1)
                if (iToNeuron == -1 && iFromNeuron >= 0) {
                    // This is an axon (output synapse) - weight MUST be 1.0 per specification
                    if (rWeight != 1.0) {
                        // Record invalid axon weight but continue importing with corrected weight
                        ostringstream oss;
                        oss << "Invalid axon weight " << rWeight << " for connection from neuron " 
                            << iFromNeuron << " to output (should be 1.0)";
                        invalidAxonConnections.push_back(oss.str());
                    }
                    Neuron* pSourceNeuron = findNeuronByGlobalIndex(network, iFromNeuron);
                    if (pSourceNeuron) {
                        // Create synapse with corrected weight (constructor will force 1.0 anyway)
                        unique_ptr<Synapse> synapse(new Synapse(1.0, pSourceNeuron, nullptr, true));
                        pSourceNeuron->addOutputSynapse(move(synapse));
                    }
                    continue;
                }
                
                // Handle inter-neuron connections
                if (iFromNeuron >= 0 && iToNeuron >= 0) {
                    // For inter-neuron connections, connectTo() will create both axon (weight 1.0) 
                    // and dendrite (weight from file). The actual connection weight goes to the dendrite.
                    Neuron* pSourceNeuron = findNeuronByGlobalIndex(network, iFromNeuron);
                    Neuron* pTargetNeuron = findNeuronByGlobalIndex(network, iToNeuron);
                    
                    if (pSourceNeuron && pTargetNeuron) {
                        pSourceNeuron->connectTo(*pTargetNeuron, rWeight);
                    }
                }
            }
        }
    }
    
    // If we found invalid axon weights, cache the error information
    if (!invalidAxonConnections.empty()) {
        ostringstream errorMsg;
        errorMsg << "File contains invalid axon weights (axon weights must be 1.0 per specification):\n";
        for (const auto& error : invalidAxonConnections) {
            errorMsg << "  - " << error << "\n";
        }
        errorMsg << "Network has been imported with corrected weights, but original file is non-compliant.";
        network.setImportError(errorMsg.str());
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
void ANNImporter::skipWhitespaceAndComments(ifstream& file) {
    char ch;
    while (file.get(ch)) {
        if (isspace(ch)) {
            continue; // Skip whitespace
        }
        else if (ch == '#') {
            // Skip comment line
            string line;
            getline(file, line);
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
string ANNImporter::readToken(ifstream& file) {
    skipWhitespaceAndComments(file);
    
    string token;
    char ch;
    
    while (file.get(ch)) {
        if (isspace(ch)) {
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
unique_ptr<ActivationFunction> ANNImporter::parseActivationFunction(const string& functionName) {
    return createActivationFunction(functionName);
}

//-------------------------------------------------------------
//【函数名称】createNeuronWithActivation
//【函数功能】根据激活函数类型创建神经元
//【参数】bias：偏置值，activationType：激活函数类型
//【返回值】unique_ptr<Neuron>，创建的神经元指针
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
unique_ptr<Neuron> ANNImporter::createNeuronWithActivation(double bias, int activationType) {
    unique_ptr<Neuron> neuron(new Neuron());
    neuron->setBias(bias);
    
    // Set activation function based on type
    switch (activationType) {
        case 0:
            // Type 0 = Linear (no activation)
            neuron->setActivationFunction(createActivationFunction("Linear"));
            break;
        case 1:
            // Type 1 = Sigmoid
            neuron->setActivationFunction(createActivationFunction("Sigmoid"));
            break;
        case 2:
            // Type 2 = Tanh
            neuron->setActivationFunction(createActivationFunction("Tanh"));
            break;
        case 3:
            // Type 3 = ReLU
            neuron->setActivationFunction(createActivationFunction("ReLU"));
            break;
        default:
            // Default to linear for unknown types
            neuron->setActivationFunction(createActivationFunction("Linear"));
            break;
    }
    
    return neuron;
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
    int iCurrentIndex = 0;
    
    for (int iLayerIdx = 0; iLayerIdx < network.getLayerCount(); ++iLayerIdx) {
        Layer* layer = network.getLayer(iLayerIdx);
        if (!layer) continue;
        
        for (int iNeuronIdx = 0; iNeuronIdx < layer->getNeuronCount(); ++iNeuronIdx) {
            if (iCurrentIndex == globalIndex) {
                return layer->getNeuron(iNeuronIdx);
            }
            iCurrentIndex++;
        }
    }
    
    return nullptr;
}
