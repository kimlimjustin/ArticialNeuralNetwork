//-------------------------------------------------------------
//【文件名】FileUtils.hpp
//【功能模块和目的】文件操作相关工具函数声明
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#ifndef FileUtils_hpp
#define FileUtils_hpp

#include <string>
#include <vector>

using namespace std;

//-------------------------------------------------------------
//【类名】FileUtils
//【功能】文件操作工具类，提供路径、扩展名、文件校验等静态方法
//【说明】所有方法均为静态方法，无需实例化
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
class FileUtils {
public:
    //-------------------------------------------------------------
    //【函数名称】getFileExtension
    //【函数功能】获取文件扩展名（小写，带点）
    //【参数】filename：文件路径或名称
    //【返回值】string，扩展名
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string getFileExtension(const string& filename);
    
    //-------------------------------------------------------------
    //【函数名称】getFilenameWithoutExtension
    //【函数功能】获取去除扩展名的文件名
    //【参数】filepath：完整文件路径
    //【返回值】string，去除扩展名后的文件名
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string getFilenameWithoutExtension(const string& filepath);
    
    //-------------------------------------------------------------
    //【函数名称】getDirectoryPath
    //【函数功能】从完整文件路径中提取目录路径
    //【参数】filepath：完整文件路径
    //【返回值】string，目录路径
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string getDirectoryPath(const string& filepath);
    
    //-------------------------------------------------------------
    //【函数名称】fileExists
    //【函数功能】检查文件是否存在
    //【参数】filename：文件路径
    //【返回值】bool，文件存在且可读返回true
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static bool fileExists(const string& filename);
    
    //-------------------------------------------------------------
    //【函数名称】isDirectory
    //【函数功能】检查路径是否为目录
    //【参数】path：待检查的路径
    //【返回值】bool，路径为目录返回true
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static bool isDirectory(const string& path);
    
    //-------------------------------------------------------------
    //【函数名称】getFileSize
    //【函数功能】获取文件大小（字节数）
    //【参数】filename：文件路径
    //【返回值】long，文件大小（字节数），文件不存在返回-1
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static long getFileSize(const string& filename);
    
    //-------------------------------------------------------------
    //【函数名称】readFileToString
    //【函数功能】读取文件全部内容为字符串
    //【参数】filename：文件路径
    //【返回值】string，文件内容字符串，读取失败返回空字符串
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string readFileToString(const string& filename);
    
    //-------------------------------------------------------------
    //【函数名称】writeStringToFile
    //【函数功能】将字符串内容写入文件
    //【参数】filename：输出文件路径
    //【参数】content：待写入内容
    //【返回值】bool，写入成功返回true
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static bool writeStringToFile(const string& filename, const string& content);
    
    //-------------------------------------------------------------
    //【函数名称】normalizePath
    //【函数功能】规范化文件路径（将反斜杠转换为斜杠）
    //【参数】path：待规范化的文件路径
    //【返回值】string，规范化后的文件路径
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string normalizePath(const string& path);
    
    //-------------------------------------------------------------
    //【函数名称】hasExtension
    //【函数功能】检查文件名是否具有指定扩展名
    //【参数】filename：文件名或路径
    //【参数】extension：待检查的扩展名（带点或不带点均可）
    //【返回值】bool，文件名具有该扩展名返回true
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static bool hasExtension(const string& filename, const string& extension);
    
    //-------------------------------------------------------------
    //【函数名称】toLowerCase
    //【函数功能】将字符串转换为小写
    //【参数】str：待转换字符串
    //【返回值】string，小写转换后的字符串
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string toLowerCase(const string& str);
    
    //-------------------------------------------------------------
    //【函数名称】trim
    //【函数功能】去除字符串首尾空白
    //【参数】str：待处理字符串
    //【返回值】string，去除首尾空白后的字符串
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static string trim(const string& str);
    
    //-------------------------------------------------------------
    //【函数名称】split
    //【函数功能】按分隔符拆分字符串
    //【参数】str：待拆分字符串
    //【参数】delimiter：分隔符字符
    //【返回值】vector<string>，拆分后的字符串向量
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    static vector<string> split(const string& str, char delimiter);

private:
    //-------------------------------------------------------------
    //【函数名称】FileUtils
    //【函数功能】私有构造函数（工具类）
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    FileUtils() = delete;
    
    //-------------------------------------------------------------
    //【函数名称】FileUtils（拷贝构造）
    //【函数功能】私有拷贝构造函数（工具类）
    //【参数】other：被拷贝的FileUtils
    //【返回值】无
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    FileUtils(const FileUtils& other) = delete;
    
    //-------------------------------------------------------------
    //【函数名称】operator=
    //【函数功能】私有赋值运算符（工具类）
    //【参数】other：赋值来源FileUtils
    //【返回值】FileUtils&，禁用返回
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    FileUtils& operator=(const FileUtils& other) = delete;
    
    // Utility class - destructor not needed since class cannot be instantiated
};

#endif // FileUtils_hpp
