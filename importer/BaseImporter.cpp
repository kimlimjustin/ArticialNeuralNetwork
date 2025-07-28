//-------------------------------------------------------------
//【文件名】BaseImporter.cpp
//【功能模块和目的】神经网络模型导入基类实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "BaseImporter.h"
#include "../utils/FileUtils.h"
#include <algorithm>

//-------------------------------------------------------------
//【函数名称】isFormatSupported
//【函数功能】判断文件格式是否支持
//【参数】filename：文件名
//【返回值】bool，是否支持
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool BaseImporter::isFormatSupported(const std::string& filename) const {
    std::string extension = getFileExtension(filename);
    std::string supportedExts = getSupportedExtensions();
    
    // Convert to lowercase for comparison
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    std::transform(supportedExts.begin(), supportedExts.end(), supportedExts.begin(), ::tolower);
    
    return supportedExts.find(extension) != std::string::npos;
}

//-------------------------------------------------------------
//【函数名称】getFileExtension
//【函数功能】获取文件扩展名
//【参数】filename：文件名
//【返回值】std::string，扩展名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string BaseImporter::getFileExtension(const std::string& filename) const {
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
    
    // Basic validation - can be enhanced
    return network->getLayerCount() > 0;
}