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

    struct Vec2 {
        float x, y;
        Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    };

    struct Vec3
    {
		float x, y, z;
		Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
    };

    struct Vec4 {
        float x, y, z, w;
        Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) 
            : x(x), y(y), z(z), w(w) {}
    };

} // namespace FCT
