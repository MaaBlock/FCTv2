#pragma once

namespace FCT {

    enum class DataType {
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat2,
        Mat3,
        Mat4,
        Int,
        IVec2,
        IVec3,
        IVec4,
        UInt,
        UVec2,
        UVec3,
        UVec4,
        Bool,
        BVec2,
        BVec3,
        BVec4,
    };


    enum class PipelineAttributeType {
        Position2f,
        Position3f,
        Position4f,
        Color4f,
        TexCoord2f,
        Normal3f,
        Tangent3f,
        Bitangent3f,
        Custom
    };
    size_t GetDataTypeSize(DataType type);
    const char* GetDataTypeName(DataType type);

} // namespace FCT