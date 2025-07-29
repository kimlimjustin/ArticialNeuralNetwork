//-------------------------------------------------------------
//【文件名】BaseExporter.cpp
//【功能模块和目的】神经网络模型导出基类实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "BaseExporter.hpp"
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
bool BaseExporter::isFormatSupported(const string& filename) const {
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
string BaseExporter::getFileExtension(const string& filename) const {
    return FileUtils::getFileExtension(filename);
}

//-------------------------------------------------------------
//【函数名称】validateNetworkForExport
//【函数功能】校验导出网络结构
//【参数】network：网络引用
//【返回值】bool，是否合法
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool BaseExporter::validateNetworkForExport(const Network& network) const {
    // Basic validation - can be enhanced
    return network.getLayerCount() > 0;
}