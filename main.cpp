//-------------------------------------------------------------
//【文件名】main.cpp
//【功能模块和目的】人工神经网络应用主入口
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "interface/ConsoleInterface.hpp"
#include <iostream>
#include <exception>

//-------------------------------------------------------------
//【函数名称】main
//【函数功能】程序主入口
//【参数】无
//【返回值】int，程序退出状态码
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int main() {
    try {
        // Create and run the console interface
        ConsoleInterface interface;
        interface.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
    
    return 0;
}
