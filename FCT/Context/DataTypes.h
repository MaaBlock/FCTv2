#pragma once
#include "./Matrix.h"
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
        BatchId,
        Custom,
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

        Vec3& operator+=(const Vec3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Vec3& operator-=(const Vec3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Vec3 operator+(const Vec3& rhs) const {
            return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Vec3 operator-(const Vec3& rhs) const {
            return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
        }
		Vec3 operator*(float scalar) const {
			return Vec3(x * scalar, y * scalar, z * scalar);
		}
		float length() const {
			return std::sqrt(x * x + y * y + z * z);
		}
		Vec3 normalize() const {
			float length = std::sqrt(x * x + y * y + z * z);
			return Vec3(x / length, y / length, z / length);
		}
	};
    inline Vec3 normalize(const Vec3& v) {
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        return Vec3(v.x / length, v.y / length, v.z / length);
    }

    inline Vec3 cross(const Vec3& a, const Vec3& b) {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    inline float dot(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    struct Vec4 {
        float x, y, z, w;
        Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) 
            : x(x), y(y), z(z), w(w) {}
    };

} // namespace FCT
