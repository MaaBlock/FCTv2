#include "VertexShader.h"
#include <sstream>
#include "DataTypes.h"

namespace FCT {

VertexShader::VertexShader(VertexFactory* factory) : m_factory(factory), m_isCompiled(false)
{
}

VertexShader::~VertexShader() {}

bool VertexShader::compileFromSource(const std::string& userCode)
{
    bool isCusstomOutputEmpty = m_output.getOutputs().empty();
    generateCode();  

    std::string finalUserCode;
    if (userCode.empty() && isCusstomOutputEmpty) {
        std::stringstream ss;
        ss << "VertexOutput main(VertexInput input) {\n";
        ss << "    VertexOutput output;\n";
        for (const auto& attr : m_factory->getAttributes()) {
              ss << "    output." << attr.name << " = input." << attr.name << ";\n";
        }
        ss << "    return output;\n";
        ss << "}\n";
        finalUserCode = ss.str();
    } else if (userCode.empty() && !isCusstomOutputEmpty) {
        m_compileError = "Custom outputs defined but no user code provided. Please provide a custom vertex shader implementation.";
        return false;
    } else {
        finalUserCode = userCode;
    }

    m_source = combineCode(finalUserCode);

    return compile();
}

bool VertexShader::isPositionAttribute(PipelineAttributeType type) const {
    return type == PipelineAttributeType::Position2f ||
           type == PipelineAttributeType::Position3f ||
           type == PipelineAttributeType::Position4f;
}

PipelineResourceType VertexShader::getType() const
{
	return PipelineResourceType::VertexShader;
}

void VertexShader::generateCode()
{
    for (const auto& attr : m_factory->getAttributes()) {
        m_output.addOutput(attr.type, attr.name, attr.dataType);
    }

    std::stringstream ss;

    ss << "#version 330 core\n\n";

    std::string positionType = getPositionType();

    ss << "struct VertexInput {\n";
    for (const auto& attr : m_factory->getAttributes()) {
        if (isPositionAttribute(attr.type)) {
            ss << "    " << positionType << " " << attr.name << ";\n";
        } else {
            ss << "    " << GetDataTypeName(attr.dataType) << " " << attr.name << ";\n";
        }
    }
    ss << "};\n\n";

    ss << "struct VertexOutput {\n";
    for (const auto& output : m_output.getOutputs()) {
        ss << "    " << GetDataTypeName(output.dataType) << " " << output.name << ";\n";
    }
    ss << "};\n\n";

    ss << "VertexOutput fct_user_main(VertexInput input);\n\n";

    int locationCounter = 0;
    for (const auto& attr : m_factory->getAttributes()) {
        if (isPositionAttribute(attr.type)) {
            ss << "layout (location = " << locationCounter << ") in " << positionType << " in_" << attr.name << ";\n";
        } else {
            ss << "layout (location = " << locationCounter << ") in " << GetDataTypeName(attr.dataType) << " in_" << attr.name << ";\n";
        }
        locationCounter++;
    }

    for (const auto& output : m_output.getOutputs()) {
        if (!isPositionAttribute(output.type)) {
            ss << "out " << GetDataTypeName(output.dataType) << " out_" << output.name << ";\n";
        }
    }
    ss << "\nvoid main() {\n";
    ss << "    VertexInput input;\n";
    for (const auto& output : m_output.getOutputs()) {
        ss << "    input." << output.name << " = in_" << output.name << ";\n";
    }
    ss << "    VertexOutput output = fct_user_main(input);\n";
    for (const auto& output : m_output.getOutputs()) {
        if (isPositionAttribute(output.type)) {
            ss << "    vec4 finalPosition = vec4(output." << output.name << ", ";
            if (output.type == PipelineAttributeType::Position2f) {
                ss << "0.0, 1.0);\n";
            } else if (output.type == PipelineAttributeType::Position3f) {
                ss << "1.0);\n";
            } else { 
                ss << ");\n";
            }
            ss << "    gl_Position = finalPosition;\n";
            break; 
        }
    }
    for (const auto& output : m_output.getOutputs()) {
        if (!isPositionAttribute(output.type)) {
            ss << "    out_" << output.name << " = output." << output.name << ";\n";
        }
    }
    ss << "}\n";

    m_source = ss.str();
}

std::string VertexShader::combineCode(const std::string& userCode) const
{

    std::string modifiedUserCode = userCode;
    size_t mainPos = modifiedUserCode.find("main");
    if (mainPos != std::string::npos) {
        modifiedUserCode.replace(mainPos, 4, "fct_user_main");
    }

    return m_source + "\n" + modifiedUserCode;
}

std::string VertexShader::getCompileError() const {
    return m_compileError;
}

const std::string& VertexShader::getSource() const {
    return m_source;
}

bool VertexShader::isCompiled() const {
    return m_isCompiled;
}

void VertexShader::addCustomOutput(PipelineAttributeType type, const std::string& name, DataType dataType)
{
    m_output.addOutput(type, name, dataType);
}

std::string VertexShader::getPositionType() const {
    for (const auto& attr : m_factory->getAttributes()) {
        if (attr.name == "position") {
            switch (attr.type) {
                case PipelineAttributeType::Position2f:
                    return "vec2";
                case PipelineAttributeType::Position3f:
                    return "vec3";
                case PipelineAttributeType::Position4f:
                    return "vec4";
                default:
                    break;
            }
        }
    }
    return "vec3"; 
}

} // namespace FCT