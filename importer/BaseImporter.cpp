//-------------------------------------------------------------
//【文件名】BaseImporter.cpp
//【功能模块和目的】神经网络模型导入基类实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "BaseImporter.hpp"
#include "../utils/FileUtils.hpp"
#include <algorithm>

using namespace std;

//-------------------------------------------------------------
//【函数名称】isFormatSupported
//【函数功能】判断文件格式是否支持
//【参数】filename：文件名
//【返回值】bool，是否支持
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool BaseImporter::isFormatSupported(const string& filename) const {
    string extension = getFileExtension(filename);
    string supportedExts = getSupportedExtensions();
    
    // Convert to lowercase for comparison
    transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    transform(supportedExts.begin(), supportedExts.end(), supportedExts.begin(), ::tolower);
    
    return supportedExts.find(extension) != string::npos;
}

//-------------------------------------------------------------
//【函数名称】getFileExtension
//【函数功能】获取文件扩展名
//【参数】filename：文件名
//【返回值】string，扩展名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string BaseImporter::getFileExtension(const string& filename) const {
    return FileUtils::getFileExtension(filename);
}

//-------------------------------------------------------------
//【函数名称】validateImportedNetwork
//【函数功能】校验导入的网络结构
//【参数】network：网络指针
//【返回值】bool，是否合法
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool BaseImporter::validateImportedNetwork(const Network* network) const {
    if (!network) {
        return false;
    }
    
    // Comprehensive validation of imported network
    if (network->getLayerCount() == 0) {
        return false;
    }
    
    // Check that each layer has at least one neuron
    for (int i = 0; i < network->getLayerCount(); ++i) {
        const Layer* layer = network->getLayer(i);
        if (!layer || layer->getNeuronCount() == 0) {
            return false;
        }
    }
    
    // Use the network's built-in validation
    return network->isValid();
}