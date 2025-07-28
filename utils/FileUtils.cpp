//-------------------------------------------------------------
//【文件名】FileUtils.cpp
//【功能模块和目的】文件操作相关工具函数实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "FileUtils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

//-------------------------------------------------------------
//【函数名称】getFileExtension
//【函数功能】获取文件扩展名（小写，带点）
//【参数】filename：文件路径或名称
//【返回值】std::string，扩展名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::getFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return "";
    }
    
    std::string extension = filename.substr(dotPos);
    return toLowerCase(extension);
}

//-------------------------------------------------------------
//【函数名称】getFilenameWithoutExtension
//【函数功能】获取去除扩展名的文件名
//【参数】filepath：完整文件路径
//【返回值】std::string，去除扩展名后的文件名
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::getFilenameWithoutExtension(const std::string& filepath) {
    // Extract filename from path
    size_t lastSlash = filepath.find_last_of("/\\");
    std::string filename = (lastSlash == std::string::npos) ? filepath : filepath.substr(lastSlash + 1);
    
    // Remove extension
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return filename;
    }
    
    return filename.substr(0, dotPos);
}

//-------------------------------------------------------------
//【函数名称】getDirectoryPath
//【函数功能】获取文件所在目录路径
//【参数】filepath：完整文件路径
//【返回值】std::string，目录路径
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::getDirectoryPath(const std::string& filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return "."; // Current directory
    }
    
    return filepath.substr(0, lastSlash);
}

//-------------------------------------------------------------
//【函数名称】fileExists
//【函数功能】检查文件是否存在
//【参数】filename：文件路径或名称
//【返回值】bool，存在返回true，否则返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool FileUtils::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

//-------------------------------------------------------------
//【函数名称】isDirectory
//【函数功能】判断路径是否为目录
//【参数】path：文件或目录路径
//【返回值】bool，是目录返回true，否则返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool FileUtils::isDirectory(const std::string& path) {
    // Basic check - in a full implementation, would use system calls
    // For now, assume paths ending with / or \ are directories
    if (path.empty()) {
        return false;
    }
    
    char lastChar = path.back();
    return (lastChar == '/' || lastChar == '\\');
}

//-------------------------------------------------------------
//【函数名称】getFileSize
//【函数功能】获取文件大小
//【参数】filename：文件路径或名称
//【返回值】long，文件大小（字节），失败返回-1
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
long FileUtils::getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return -1;
    }
    
    return static_cast<long>(file.tellg());
}

//-------------------------------------------------------------
//【函数名称】readFileToString
//【函数功能】将文件内容读取为字符串
//【参数】filename：文件路径或名称
//【返回值】std::string，文件内容
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

//-------------------------------------------------------------
//【函数名称】writeStringToFile
//【函数功能】将字符串内容写入文件
//【参数】filename：文件路径或名称
//         content：要写入的字符串内容
//【返回值】bool，成功返回true，否则返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool FileUtils::writeStringToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return file.good();
}

//-------------------------------------------------------------
//【函数名称】normalizePath
//【函数功能】规范化路径字符串
//【参数】path：原始路径
//【返回值】std::string，规范化后的路径
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::normalizePath(const std::string& path) {
    std::string normalized = path;
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    return normalized;
}

//-------------------------------------------------------------
//【函数名称】hasExtension
//【函数功能】检查文件名是否具有特定扩展名
//【参数】filename：文件路径或名称
//         extension：要检查的扩展名
//【返回值】bool，具有指定扩展名返回true，否则返回false
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
bool FileUtils::hasExtension(const std::string& filename, const std::string& extension) {
    std::string fileExt = getFileExtension(filename);
    std::string checkExt = extension;
    
    // Add dot if not present
    if (!checkExt.empty() && checkExt[0] != '.') {
        checkExt = "." + checkExt;
    }
    
    return toLowerCase(fileExt) == toLowerCase(checkExt);
}

//-------------------------------------------------------------
//【函数名称】toLowerCase
//【函数功能】将字符串转换为小写
//【参数】str：原始字符串
//【返回值】std::string，小写字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

//-------------------------------------------------------------
//【函数名称】trim
//【函数功能】去除字符串首尾的空白字符
//【参数】str：原始字符串
//【返回值】std::string，去除首尾空白后的字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string FileUtils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

//-------------------------------------------------------------
//【函数名称】split
//【函数功能】按分隔符拆分字符串
//【参数】str：原始字符串
//         delimiter：分隔符字符
//【返回值】std::vector<std::string>，拆分后的字符串向量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::vector<std::string> FileUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}
