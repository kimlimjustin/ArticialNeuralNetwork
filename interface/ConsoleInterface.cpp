//-------------------------------------------------------------
//【文件名】ConsoleInterface.cpp
//【功能模块和目的】控制台交互界面实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "ConsoleInterface.h"
#include "../controller/NetworkController.h"
#include <iostream>
#include <sstream>
#include <limits>

//-------------------------------------------------------------
//【函数名称】ConsoleInterface
//【函数功能】构造函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
ConsoleInterface::ConsoleInterface() : m_isRunning(false) {
}
//-------------------------------------------------------------
//【函数名称】~ConsoleInterface
//【函数功能】析构函数
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
ConsoleInterface::~ConsoleInterface() {
}
//-------------------------------------------------------------
//【函数名称】run
//【函数功能】主循环，处理用户交互
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::run() {
    m_isRunning = true;
    displayInfo("Welcome to Artificial Neural Network System");
    displayInfo("=========================================");
    
    while (m_isRunning) {
        try {
            displayMainMenu();
            int choice = getIntInput("Enter your choice: ");
            
            switch (choice) {
                case 1:
                    handleImportNetwork();
                    break;
                case 2:
                    handleExportNetwork();
                    break;
                case 3:
                    handleModifyNetwork();
                    break;
                case 4:
                    handleDisplayStatistics();
                    break;
                case 5:
                    handleValidateNetwork();
                    break;
                case 6:
                    handleInference();
                    break;
                case 0:
                    exit();
                    break;
                default:
                    displayError("Invalid choice. Please try again.");
                    break;
            }
        }
        catch (const std::exception& e) {
            displayError(std::string("Error: ") + e.what());
        }
    }
    
    displayInfo("Thank you for using ANN System!");
}

//-------------------------------------------------------------
//【函数名称】exit
//【函数功能】退出程序，设置运行标志为false
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::exit() {
    m_isRunning = false;
}

//-------------------------------------------------------------
//【函数名称】displayMainMenu
//【函数功能】显示主菜单选项
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displayMainMenu() const {
    std::cout << "\n=== Main Menu ===" << std::endl;
    std::cout << "1. Import Network" << std::endl;
    std::cout << "2. Export Network" << std::endl;
    std::cout << "3. Modify Network" << std::endl;
    std::cout << "4. Display Statistics" << std::endl;
    std::cout << "5. Validate Network" << std::endl;
    std::cout << "6. Run Inference" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

//-------------------------------------------------------------
//【函数名称】displayModificationMenu
//【函数功能】显示网络修改菜单选项
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displayModificationMenu() const {
    std::cout << "\n=== Modification Menu ===" << std::endl;
    std::cout << "1. Layer Operations" << std::endl;
    std::cout << "2. Neuron Operations" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;
}

//-------------------------------------------------------------
//【函数名称】handleImportNetwork
//【函数功能】处理导入网络操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleImportNetwork() {
    std::string filename = getStringInput("Enter filename to import: ");
    
    NetworkController& controller = NetworkController::getInstance();
    if (controller.importNetwork(filename)) {
        displaySuccess("Network imported successfully!");
    } else {
        displayError("Failed to import network. Please check the file format and path.");
    }
}

//-------------------------------------------------------------
//【函数名称】handleExportNetwork
//【函数功能】处理导出网络操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleExportNetwork() {
    NetworkController& controller = NetworkController::getInstance();
    if (!controller.hasNetwork()) {
        displayError("No network loaded. Please import a network first.");
        return;
    }
    
    std::string filename = getStringInput("Enter filename to export: ");
    
    if (controller.exportNetwork(filename)) {
        displaySuccess("Network exported successfully!");
    } else {
        displayError("Failed to export network. Please check the file path and permissions.");
    }
}

//-------------------------------------------------------------
//【函数名称】handleModifyNetwork
//【函数功能】处理网络修改操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleModifyNetwork() {
    NetworkController& controller = NetworkController::getInstance();
    if (!controller.hasNetwork()) {
        displayError("No network loaded. Please import a network first.");
        return;
    }
    
    bool modifying = true;
    while (modifying) {
        displayModificationMenu();
        int choice = getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                handleLayerOperations();
                break;
            case 2:
                handleNeuronOperations();
                break;
            case 0:
                modifying = false;
                break;
            default:
                displayError("Invalid choice. Please try again.");
                break;
        }
    }
}

//-------------------------------------------------------------
//【函数名称】handleDisplayStatistics
//【函数功能】处理显示统计信息操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleDisplayStatistics() {
    NetworkController& controller = NetworkController::getInstance();
    if (!controller.hasNetwork()) {
        displayError("No network loaded. Please import a network first.");
        return;
    }
    
    std::string stats = controller.getNetworkStatistics();
    displayInfo("Network Statistics:");
    std::cout << stats << std::endl;
}

//-------------------------------------------------------------
//【函数名称】handleValidateNetwork
//【函数功能】处理网络验证操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleValidateNetwork() {
    NetworkController& controller = NetworkController::getInstance();
    if (!controller.hasNetwork()) {
        displayError("No network loaded. Please import a network first.");
        return;
    }
    
    if (controller.validateNetwork()) {
        displaySuccess("Network is valid and ready for inference!");
    } else {
        displayError("Network validation failed. Please check network structure.");
    }
}

//-------------------------------------------------------------
//【函数名称】handleInference
//【函数功能】处理推理操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleInference() {
    NetworkController& controller = NetworkController::getInstance();
    if (!controller.hasNetwork()) {
        displayError("No network loaded. Please import a network first.");
        return;
    }
    
    if (!controller.validateNetwork()) {
        displayError("Network is not valid. Cannot perform inference.");
        return;
    }
    
    int inputSize = controller.getInputSize();
    if (inputSize <= 0) {
        displayError("Invalid network input size.");
        return;
    }
    
    // Display information about the expected input format
    displayInfo("Network expects " + std::to_string(inputSize) + " input values.");
    
    // Generate and display an example of input values
    std::cout << "\nExample input values: ";
    for (int i = 0; i < inputSize; ++i) {
        if (i == 0) {
            std::cout << "1.0";
        } else if (i == 1) {
            std::cout << " 0.5";
        } else if (i == 2) {
            std::cout << " -0.3";
        } else {
            std::cout << " " << (i % 2 == 0 ? "0.0" : "1.0");
        }
    }
    std::cout << std::endl;
    std::cout << "Note: Values should be separated by spaces." << std::endl;
    std::cout << std::endl;
    
    std::vector<double> inputs = getDoubleVector("Enter input values", inputSize);
    
    try {
        std::vector<double> outputs = controller.runInference(inputs);
        
        displaySuccess("Inference completed!");
        std::cout << "Output values: ";
        for (size_t i = 0; i < outputs.size(); ++i) {
            std::cout << outputs[i];
            if (i < outputs.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        displayError(std::string("Inference failed: ") + e.what());
    }
}

//-------------------------------------------------------------
//【函数名称】handleLayerOperations
//【函数功能】处理层管理操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleLayerOperations() {
    NetworkController& controller = NetworkController::getInstance();
    
    std::cout << "\n=== Layer Operations ===" << std::endl;
    std::cout << "1. List all layers" << std::endl;
    std::cout << "2. Add layer" << std::endl;
    std::cout << "3. Delete layer" << std::endl;
    std::cout << "0. Back" << std::endl;
    
    int choice = getIntInput("Enter your choice: ");
    
    switch (choice) {
        case 1: {
            std::string layerInfo = controller.getLayerInformation();
            std::cout << layerInfo << std::endl;
            break;
        }
        case 2: {
            if (controller.addLayer()) {
                displaySuccess("Layer added successfully!");
            } else {
                displayError("Failed to add layer.");
            }
            break;
        }
        case 3: {
            int layerIndex = getIntInput("Enter layer index to delete: ");
            if (controller.deleteLayer(layerIndex)) {
                displaySuccess("Layer deleted successfully!");
            } else {
                displayError("Failed to delete layer. Check layer index.");
            }
            break;
        }
        case 0:
            break;
        default:
            displayError("Invalid choice.");
            break;
    }
}

//-------------------------------------------------------------
//【函数名称】handleNeuronOperations
//【函数功能】处理神经元管理操作
//【参数】无
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::handleNeuronOperations() {
    NetworkController& controller = NetworkController::getInstance();
    
    std::cout << "\n=== Neuron Operations ===" << std::endl;
    std::cout << "1. List neurons in layer" << std::endl;
    std::cout << "2. Modify neuron bias" << std::endl;
    std::cout << "3. Show neuron connections" << std::endl;
    std::cout << "4. Delete neuron" << std::endl;
    std::cout << "5. Connect neurons" << std::endl;
    std::cout << "0. Back" << std::endl;
    
    int choice = getIntInput("Enter your choice: ");
    
    switch (choice) {
        case 1: {
            int layerIndex = getIntInput("Enter layer index: ");
            std::string neuronInfo = controller.getNeuronInformation(layerIndex);
            std::cout << neuronInfo << std::endl;
            break;
        }
        case 2: {
            int layerIndex = getIntInput("Enter layer index: ");
            int neuronIndex = getIntInput("Enter neuron index: ");
            double bias = getDoubleInput("Enter new bias value: ");
            if (controller.modifyNeuronBias(layerIndex, neuronIndex, bias)) {
                displaySuccess("Neuron bias modified successfully!");
            } else {
                displayError("Failed to modify neuron bias. Check indices.");
            }
            break;
        }
        case 3: {
            int layerIndex = getIntInput("Enter layer index: ");
            int neuronIndex = getIntInput("Enter neuron index: ");
            std::string connectionInfo = controller.getNeuronConnections(layerIndex, neuronIndex);
            std::cout << connectionInfo << std::endl;
            break;
        }
        case 4: {
            int layerIndex = getIntInput("Enter layer index: ");
            int neuronIndex = getIntInput("Enter neuron index: ");
            if (controller.deleteNeuron(layerIndex, neuronIndex)) {
                displaySuccess("Neuron deleted successfully!");
            } else {
                displayError("Failed to delete neuron. Check indices.");
            }
            break;
        }
        case 5: {
            int fromLayer = getIntInput("Enter source layer index: ");
            int fromNeuron = getIntInput("Enter source neuron index: ");
            int toLayer = getIntInput("Enter target layer index: ");
            int toNeuron = getIntInput("Enter target neuron index: ");
            double weight = getDoubleInput("Enter connection weight: ");
            
            if (controller.connectNeurons(fromLayer, fromNeuron, toLayer, toNeuron, weight)) {
                displaySuccess("Neurons connected successfully!");
            } else {
                displayError("Failed to connect neurons. Check indices and adjacency.");
            }
            break;
        }
        case 0:
            break;
        default:
            displayError("Invalid choice.");
            break;
    }
}

//-------------------------------------------------------------
//【函数名称】getStringInput
//【函数功能】获取用户字符串输入
//【参数】prompt：提示信息
//【返回值】std::string，用户输入的字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::string ConsoleInterface::getStringInput(const std::string& prompt) const {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

//-------------------------------------------------------------
//【函数名称】getIntInput
//【函数功能】获取用户整数输入
//【参数】prompt：提示信息
//【返回值】int，用户输入的整数
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
int ConsoleInterface::getIntInput(const std::string& prompt) const {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        std::istringstream iss(input);
        int value;
        if (iss >> value && iss.eof()) {
            return value;
        }
        
        displayError("Invalid integer. Please try again.");
    }
}

//-------------------------------------------------------------
//【函数名称】getDoubleInput
//【函数功能】获取用户浮点数输入
//【参数】prompt：提示信息
//【返回值】double，用户输入的浮点数
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
double ConsoleInterface::getDoubleInput(const std::string& prompt) const {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        std::istringstream iss(input);
        double value;
        if (iss >> value && iss.eof()) {
            return value;
        }
        
        displayError("Invalid number. Please try again.");
    }
}

//-------------------------------------------------------------
//【函数名称】getDoubleVector
//【函数功能】获取用户浮点数向量输入
//【参数】prompt：提示信息，size：预期数值个数
//【返回值】std::vector<double>，浮点数向量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
std::vector<double> ConsoleInterface::getDoubleVector(const std::string& prompt, int size) const {
    std::vector<double> values;
    std::cout << prompt << " (" << size << " values separated by spaces): ";
    
    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss(input);
    
    double value;
    while (iss >> value && values.size() < static_cast<size_t>(size)) {
        values.push_back(value);
    }
    
    if (values.size() != static_cast<size_t>(size)) {
        displayError("Invalid number of values. Expected " + std::to_string(size));
        return getDoubleVector(prompt, size); // Retry
    }
    
    return values;
}

//-------------------------------------------------------------
//【函数名称】displayError
//【函数功能】显示错误信息
//【参数】message：错误信息
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displayError(const std::string& message) const {
    std::cerr << "[ERROR] " << message << std::endl;
}

//-------------------------------------------------------------
//【函数名称】displaySuccess
//【函数功能】显示成功信息
//【参数】message：成功信息
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displaySuccess(const std::string& message) const {
    std::cout << "[SUCCESS] " << message << std::endl;
}

//-------------------------------------------------------------
//【函数名称】displayInfo
//【函数功能】显示信息
//【参数】message：信息内容
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displayInfo(const std::string& message) const {
    std::cout << "[INFO] " << message << std::endl;
}


