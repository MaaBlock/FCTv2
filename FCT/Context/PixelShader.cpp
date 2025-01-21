#include "../headers.h"
#include <sstream>

namespace FCT {

    PixelShader::PixelShader(const ShaderOutput& vertexOutput)
        : m_isCompiled(false), m_vertexOutput(vertexOutput)
    {
    }

    PixelShader::~PixelShader() {}

    bool PixelShader::compileFromSource(const std::string& userCode)
    {
        std::string finalUserCode = userCode.empty() ? generateDefaultCode() : userCode;
        m_source = combineCode(finalUserCode);
        return compile();
    }

    std::string PixelShader::generateDefaultCode() const
    {
        std::stringstream ss;
        ss << "PixelOutput main(PixelInput input) {\n";
        ss << "    PixelOutput output;\n";

        bool hasColorAttribute = false;
        for (const auto& output : m_vertexOutput.getOutputs()) {
            if (output.type == PipelineAttributeType::Color4f) {
                hasColorAttribute = true;
            }
            ss << "    output." << output.name << " = input." << output.name << ";\n";
        }

        if (!hasColorAttribute) {
            ss << "    output.color = vec4(1.0, 0.647, 0.0, 1.0);\n";
        }

        ss << "    return output;\n";
        ss << "}\n";
        return ss.str();
    }

    std::string PixelShader::combineCode(const std::string& userCode) const
    {
        std::stringstream ss;
        ss << "#version 330 core\n\n";

        ss << "struct PixelInput {\n";
        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "    " << GetDataTypeName(output.dataType) << " " << output.name << ";\n";
        }
        ss << "};\n\n";

        ss << "struct PixelOutput {\n";
        bool hasColorAttribute = false;
        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "    " << GetDataTypeName(output.dataType) << " " << output.name << ";\n";
            if (output.type == PipelineAttributeType::Color4f) {
                hasColorAttribute = true;
            }
        }
        if (!hasColorAttribute) {
            ss << "    vec4 color;\n";
        }
        ss << "};\n\n";

        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "in " << GetDataTypeName(output.dataType) << " in_" << output.name << ";\n";
        }

        ss << "\nout vec4 FragColor;\n\n";

        ss << "PixelOutput main(PixelInput input);\n\n";

        ss << "void FCT_Main() {\n";
        ss << "    PixelInput input;\n";
        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "    input." << output.name << " = in_" << output.name << ";\n";
        }
        ss << "    PixelOutput output = main(input);\n";
        if (hasColorAttribute) {
            for (const auto& output : m_vertexOutput.getOutputs()) {
                if (output.type == PipelineAttributeType::Color4f) {
                    ss << "    FragColor = output." << output.name << ";\n";
                    break;
                }
            }
        }
        else {
            ss << "    FragColor = output.color;\n";
        }
        ss << "}\n\n";

        ss << userCode;

        return ss.str();
    }
    std::string PixelShader::getCompileError() const {
        return m_compileError;
    }

    const std::string& PixelShader::getSource() const {
        return m_source;
    }

    bool PixelShader::isCompiled() const {
        return m_isCompiled;
    }

} // namespace FCT