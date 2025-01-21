#pragma once

namespace FCT {

enum class VertexAttributeType {
    Position,
    Normal,
    Color,
    TexCoord,
    Tangent,
    Bitangent,
};

struct Vec2 {
    float x, y;
    Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

struct Vec4 {
    float x, y, z, w;
    Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
};


} // namespace FCT