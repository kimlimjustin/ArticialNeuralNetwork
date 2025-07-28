//-------------------------------------------------------------
//【文件名】ANNExporter.h
//【功能模块和目的】ANN格式神经网络模型导出器声明
//【开发者及日期】林钲凯 2025-07-27
//【更改记录】
//-------------------------------------------------------------

#ifndef ANN_EXPORTER_H
#define ANN_EXPORTER_H

#include "BaseExporter.h"
#include <fstream>

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
    bool writeNetworkHeader(std::ofstream& file, const Network& network);
    
    /**
     * @brief Write layer information
     * @param file Output file stream
     * @param network Network to export
     * @return True if layers written successfully
     */
    bool writeLayerInformation(std::ofstream& file, const Network& network);
    
    /**
     * @brief Write neuron information for a layer
     * @param file Output file stream
     * @param layer Layer to export
     * @return True if neurons written successfully
     */
    bool writeNeuronInformation(std::ofstream& file, const Layer& layer);
    
    /**
     * @brief Write synapse connections
     * @param file Output file stream
     * @param network Network to export
     * @return True if connections written successfully
     */
    bool writeConnections(std::ofstream& file, const Network& network);
    
    /**
     * @brief Write comment to file
     * @param file Output file stream
     * @param comment Comment text
     */
    void writeComment(std::ofstream& file, const std::string& comment);
    
    /**
     * @brief Get activation function name
     * @param activationFunction Activation function pointer
     * @return Function name string
     */
    std::string getActivationFunctionName(const ActivationFunction* activationFunction);

public:
    /**
     * @brief Constructor
     */
    ANNExporter();
    
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
    bool exportNetwork(const Network& network, const std::string& filename) override;
    
    /**
     * @brief Get supported file extensions
     * @return ".ann"
     */
    std::string getSupportedExtensions() const override;
    
    /**
     * @brief Get exporter name
     * @return "ANN Exporter"
     */
    std::string getExporterName() const override;
};

#endif // ANN_EXPORTER_H
