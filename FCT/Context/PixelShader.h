#pragma once
#include <string>
#include "../MutilThreadBase/RefCount.h"
#include "./ShaderOutput.h"

namespace FCT {
    class PixelShader : public RefCount {
    public:
        PixelShader(const ShaderOutput& vertexOutput);
        virtual ~PixelShader();
        bool compileFromSource(const std::string& source);
        std::string getCompileError() const;
        const std::string& getSource() const;
        bool isCompiled() const;
        virtual bool compile() = 0;

    protected:
        std::string m_source;
        std::string m_compileError;
        bool m_isCompiled;
        const ShaderOutput& m_vertexOutput;
        std::string generateDefaultCode() const;
        std::string combineCode(const std::string& userCode) const;
    };
} // namespace FCT