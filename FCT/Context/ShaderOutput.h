#pragma once
#include <string>
#include <vector>
#include "VertexFactory.h" 

namespace FCT {

class ShaderOutput {
public:
    struct OutputVariable {
        PipelineAttributeType type;
        std::string name;
        DataType dataType;

        OutputVariable(PipelineAttributeType t, const std::string& n, DataType dt)
            : type(t), name(n), dataType(dt) {}
    };

    void addOutput(PipelineAttributeType type, const std::string& name, DataType dataType);
    const std::vector<OutputVariable>& getOutputs() const { return m_outputs; }
    std::string generateStructDefinition(const std::string& structName) const;
    std::string generateOutputDeclarations() const;
    std::string generateOutputAssignments(const std::string& outputVarName) const;

private:
    std::vector<OutputVariable> m_outputs;
};

} // namespace FCT
