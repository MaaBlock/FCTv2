#pragma once
namespace FCT {
    enum class PipelineResourceType {
        VertexShader,
        PixelShader,
        VertexBuffer,
        IndexBuffer,
        DepthStencilState,
        RasterizerState,
        BlendState,
        Texture,
    };
    class IPipelineResource {
    public:
        virtual ~IPipelineResource() {};

        virtual PipelineResourceType getType() const = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

}// namespace FCT
