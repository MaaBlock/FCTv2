#pragma once
#include <string>
#include "../MutilThreadBase/RefCount.h"
#include "./VertexFactory.h"
#include "./ShaderOutput.h"

namespace FCT {

class VertexShader : public RefCount {
public:
    VertexShader(VertexFactory* factory);
    virtual ~VertexShader();
    bool compileFromSource(const std::string& source);
    std::string getCompileError() const;
    const std::string& getSource() const;
    bool isCompiled() const;
    virtual bool compile() = 0;
    void addCustomOutput(PipelineAttributeType type, const std::string& name, DataType dataType);
    const ShaderOutput& getOutput() const { return m_output; }

protected:
    VertexFactory* m_factory;
    std::string m_source;
    std::string m_compileError;
    bool m_isCompiled;
    ShaderOutput m_output;

    void generateCode();
    std::string combineCode(const std::string& userCode) const;
    std::string getPositionType() const;
    bool isPositionAttribute(PipelineAttributeType type) const;
};
} // namespace FCT
