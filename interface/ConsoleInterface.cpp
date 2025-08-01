//-------------------------------------------------------------
//【文件名】ConsoleInterface.cpp
//【功能模块和目的】控制台交互界面实现
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#include "ConsoleInterface.hpp"
#include "../controller/NetworkController.hpp"
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

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
ConsoleInterface::~ConsoleInterface() = default;
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
    displayInfo("=========================================");        while (m_isRunning) {
        try {
            displayMainMenu();
            int iChoice = getIntInput("Enter your choice: ");
            
            switch (iChoice) {
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
        catch (const exception& e) {
            displayError(string("Error: ") + e.what());
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
    cout << "\n=== Main Menu ===" << endl;
    cout << "1. Import Network" << endl;
    cout << "2. Export Network" << endl;
    cout << "3. Network Details and Modify Network" << endl;
    cout << "4. Display Statistics" << endl;
    cout << "5. Validate Network" << endl;
    cout << "6. Run Inference" << endl;
    cout << "0. Exit" << endl;
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
    cout << "\n=== Modification Menu ===" << endl;
    cout << "1. Layer Operations" << endl;
    cout << "2. Neuron Operations" << endl;
    cout << "0. Back to Main Menu" << endl;
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
    string filename = getStringInput("Enter filename to import: ");
    
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
    
    string filename = getStringInput("Enter filename to export: ");
    
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
    
    bool bModifying = true;
    while (bModifying) {
        displayModificationMenu();
        int iChoice = getIntInput("Enter your choice: ");
        
        switch (iChoice) {
            case 1:
                handleLayerOperations();
                break;
            case 2:
                handleNeuronOperations();
                break;
            case 0:
                bModifying = false;
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
    
    string stats = controller.getNetworkStatistics();
    displayInfo("Network Statistics:");
    cout << stats << endl;
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
    
    // Get detailed validation information
    string validationDetails = controller.getValidationDetails();
    cout << "[INFO] " << validationDetails << endl;
    
    if (controller.validateNetwork()) {
        displaySuccess("Network validation completed - see details above.");
    } else {
        displayError("Network validation failed - see details above.");
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
    
    int iInputSize = controller.getInputSize();
    if (iInputSize <= 0) {
        displayError("Invalid network input size.");
        return;
    }
    
    // Display information about the expected input format
    displayInfo("Network expects " + to_string(iInputSize) + " input values.");
    
    // Generate and display an example of input values
    cout << "\nExample input values: ";
    for (int iInputIdx = 0; iInputIdx < iInputSize; ++iInputIdx) {
        if (iInputIdx == 0) {
            cout << "1.0";
        } else if (iInputIdx == 1) {
            cout << " 0.5";
        } else if (iInputIdx == 2) {
            cout << " -0.3";
        } else {
            cout << " " << (iInputIdx % 2 == 0 ? "0.0" : "1.0");
        }
    }
    cout << endl;
    cout << "Note: Values should be separated by spaces." << endl;
    cout << endl;
    
    vector<double> inputs = getDoubleVector("Enter input values", iInputSize);
    
    try {
        vector<double> outputs = controller.runInference(inputs);
        
        displaySuccess("Inference completed!");
        cout << "Output values: ";
        for (size_t i = 0; i < outputs.size(); ++i) {
            cout << outputs[i];
            if (i < outputs.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    catch (const exception& e) {
        displayError(string("Inference failed: ") + e.what());
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
    
    cout << "\n=== Layer Operations ===" << endl;
    cout << "1. List all layers" << endl;
    cout << "2. Add layer" << endl;
    cout << "3. Delete layer" << endl;
    cout << "0. Back" << endl;
    
    int iChoice = getIntInput("Enter your choice: ");
    
    switch (iChoice) {
        case 1: {
            string layerInfo = controller.getLayerInformation();
            cout << layerInfo << endl;
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
            int iLayerIndex = getIntInput("Enter layer index to delete: ");
            if (controller.deleteLayer(iLayerIndex)) {
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
    
    cout << "\n=== Neuron Operations ===" << endl;
    cout << "1. List neurons in layer" << endl;
    cout << "2. Modify neuron bias" << endl;
    cout << "3. Show neuron connections" << endl;
    cout << "4. Delete neuron" << endl;
    cout << "5. Connect neurons" << endl;
    cout << "0. Back" << endl;
    
    int iChoice = getIntInput("Enter your choice: ");
    
    switch (iChoice) {
        case 1: {
            int iLayerIndex = getIntInput("Enter layer index: ");
            string neuronInfo = controller.getNeuronInformation(iLayerIndex);
            cout << neuronInfo << endl;
            break;
        }
        case 2: {
            int iLayerIndex = getIntInput("Enter layer index: ");
            int iNeuronIndex = getIntInput("Enter neuron index: ");
            double rBias = getDoubleInput("Enter new bias value: ");
            if (controller.modifyNeuronBias(iLayerIndex, iNeuronIndex, rBias)) {
                displaySuccess("Neuron bias modified successfully!");
            } else {
                displayError("Failed to modify neuron bias. Check indices.");
            }
            break;
        }
        case 3: {
            int iLayerIndex = getIntInput("Enter layer index: ");
            int iNeuronIndex = getIntInput("Enter neuron index: ");
            string connectionInfo = controller.getNeuronConnections(iLayerIndex, iNeuronIndex);
            cout << connectionInfo << endl;
            break;
        }
        case 4: {
            int iLayerIndex = getIntInput("Enter layer index: ");
            int iNeuronIndex = getIntInput("Enter neuron index: ");
            if (controller.deleteNeuron(iLayerIndex, iNeuronIndex)) {
                displaySuccess("Neuron deleted successfully!");
            } else {
                displayError("Failed to delete neuron. Check indices.");
            }
            break;
        }
        case 5: {
            int iFromLayer = getIntInput("Enter source layer index: ");
            int iFromNeuron = getIntInput("Enter source neuron index: ");
            int iToLayer = getIntInput("Enter target layer index: ");
            int iToNeuron = getIntInput("Enter target neuron index: ");
            double rWeight = getDoubleInput("Enter connection weight: ");
            
            if (controller.connectNeurons(iFromLayer, iFromNeuron, iToLayer, iToNeuron, rWeight)) {
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
//【返回值】string，用户输入的字符串
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
string ConsoleInterface::getStringInput(const string& prompt) const {
    cout << prompt;
    string input;
    getline(cin, input);
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
int ConsoleInterface::getIntInput(const string& prompt) const {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        
        istringstream iss(input);
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
double ConsoleInterface::getDoubleInput(const string& prompt) const {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        
        istringstream iss(input);
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
//【返回值】vector<double>，浮点数向量
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
vector<double> ConsoleInterface::getDoubleVector(const string& prompt, int size) const {
    vector<double> values;
    cout << prompt << " (" << size << " values separated by spaces): ";
    
    string input;
    getline(cin, input);
    istringstream iss(input);
    
    double value;
    while (iss >> value && values.size() < static_cast<size_t>(size)) {
        values.push_back(value);
    }
    
    if (values.size() != static_cast<size_t>(size)) {
        displayError("Invalid number of values. Expected " + to_string(size));
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
void ConsoleInterface::displayError(const string& message) const {
    cerr << "[ERROR] " << message << endl;
}

//-------------------------------------------------------------
//【函数名称】displaySuccess
//【函数功能】显示成功信息
//【参数】message：成功信息
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displaySuccess(const string& message) const {
    cout << "[SUCCESS] " << message << endl;
}

//-------------------------------------------------------------
//【函数名称】displayInfo
//【函数功能】显示信息
//【参数】message：信息内容
//【返回值】无
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
void ConsoleInterface::displayInfo(const string& message) const {
    cout << "[INFO] " << message << endl;
}


