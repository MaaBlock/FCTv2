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
        ss << "#extension GL_ARB_shading_language_420pack : enable\n";
        ss << "layout(binding = 3) uniform sampler2D mainTexture;\n\n";
        ss << "struct PixelInput {\n";
        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "    " << GetDataTypeName(output.dataType) << " " << output.name << ";\n";
        }
        ss << "};\n\n";

        ss << "struct PixelOutput {\n";
        bool hasColorAttribute = false;
        bool hasTexCoordAttribute = false;
        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "    " << GetDataTypeName(output.dataType) << " " << output.name << ";\n";
            if (output.type == PipelineAttributeType::Color4f) {
                hasColorAttribute = true;
            }
            if (output.type == PipelineAttributeType::TexCoord2f) {
				hasTexCoordAttribute = true;
            }
        }
        if (!hasColorAttribute) {
            ss << "    vec4 color;\n";
        }
        ss << "};\n\n";

        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "in " << GetDataTypeName(output.dataType) << " vs2fs_" << output.name << ";\n";
        }
        ss << "\nout vec4 FragColor;\n\n";

        ss << "PixelOutput fct_user_main(PixelInput input);\n\n";

        ss << "void main() {\n";
        ss << "    PixelInput input;\n";
        for (const auto& output : m_vertexOutput.getOutputs()) {
            ss << "    input." << output.name << " = vs2fs_" << output.name << ";\n";
        }
        if (hasTexCoordAttribute) {
            ss << "input." << getColorName() << " = texture(mainTexture, input." << getTexCoordName() << "); \n";
        }
        ss << "    PixelOutput output = fct_user_main(input);\n";
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
        std::string modifiedUserCode = userCode;
        size_t mainPos = modifiedUserCode.find("main");
        if (mainPos != std::string::npos) {
            modifiedUserCode.replace(mainPos, 4, "fct_user_main");
        }

        ss << modifiedUserCode;

        return ss.str();
    }
    PipelineResourceType PixelShader::getType() const
    {
		return PipelineResourceType::PixelShader;
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

    std::string PixelShader::getTexCoordName() const {
        for (const auto& output : m_vertexOutput.getOutputs()) {
            if (output.type == PipelineAttributeType::TexCoord2f) {
                return output.name;
            }
        }
        return "";
    }

    std::string PixelShader::getColorName() const {
        for (const auto& output : m_vertexOutput.getOutputs()) {
            if (output.type == PipelineAttributeType::Color4f) {
                return output.name;
            }
        }
        return "color";
    }

} // namespace FCT
