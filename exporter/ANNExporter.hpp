//-------------------------------------------------------------
//【文件名】ANNExporter.hpp
//【功能模块和目的】ANN格式神经网络模型导出器声明
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#ifndef AnnExporter_hpp
#define AnnExporter_hpp

#include "BaseExporter.hpp"
#include "../model/ActivationFunction.hpp"
#include <fstream>

using namespace std;

//-------------------------------------------------------------
//【类名】ANNExporter
//【功能】ANN文件格式导出器，序列化神经网络结构
//【说明】支持ANN文件的网络、层、神经元、突触导出
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------
class ANNExporter : public BaseExporter {
private:
    /**
     * @brief Write network header information
     * @param file Output file stream
     * @param network Network to export
     * @return True if header written successfully
     */
    bool writeNetworkHeader(ofstream& file, const Network& network);
    
    /**
     * @brief Write layer information
     * @param file Output file stream
     * @param network Network to export
     * @return True if layers written successfully
     */
    bool writeLayerInformation(ofstream& file, const Network& network);
    
    /**
     * @brief Write neuron information for a layer
     * @param file Output file stream
     * @param layer Layer to export
     * @return True if neurons written successfully
     */
    bool writeNeuronInformation(ofstream& file, const Layer& layer);
    
    /**
     * @brief Write synapse connections
     * @param file Output file stream
     * @param network Network to export
     * @return True if connections written successfully
     */
    bool writeConnections(ofstream& file, const Network& network);
    
    /**
     * @brief Write comment to file
     * @param file Output file stream
     * @param comment Comment text
     */
    void writeComment(ofstream& file, const string& comment);
    
    /**
     * @brief Get activation function name
     * @param activationFunction Activation function pointer
     * @return Function name string
     */
    string getActivationFunctionName(const ActivationFunction* activationFunction) const;
    
    /**
     * @brief Find the global index of a neuron in the network
     * @param network Network to search in
     * @param targetNeuron Neuron to find
     * @return Global index of the neuron, -1 if not found
     */
    int findNeuronGlobalIndex(const Network& network, const Neuron* targetNeuron);

public:
    /**
     * @brief Constructor
     */
    ANNExporter();
    
    //-------------------------------------------------------------
    //【函数名称】ANNExporter（拷贝构造）
    //【函数功能】拷贝构造函数
    //【参数】other：被拷贝的ANN导出器
    //【返回值】无
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    ANNExporter(const ANNExporter& other) = default;
    
    //-------------------------------------------------------------
    //【函数名称】operator=
    //【函数功能】赋值运算符重载
    //【参数】other：赋值来源ANN导出器
    //【返回值】ANNExporter&，自身引用
    //【开发者及日期】林钲凯 2025-07-27
    //【更改记录】
    //-------------------------------------------------------------
    ANNExporter& operator=(const ANNExporter& other) = default;
    
    /**
     * @brief Destructor
     */
    ~ANNExporter();
    
    /**
     * @brief Export neural network to ANN file
     * @param network Network to export
     * @param filename Path to output ANN file
     * @return True if export successful
     */
    bool exportNetwork(const Network& network, const string& filename) override;
    
    /**
     * @brief Get supported file extensions
     * @return ".ann"
     */
    string getSupportedExtensions() const override;
    
    /**
     * @brief Get exporter name
     * @return "ANN Exporter"
     */
    string getExporterName() const override;
};

#endif // AnnExporter_hpp
