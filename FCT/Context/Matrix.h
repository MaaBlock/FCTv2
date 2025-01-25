#pragma once
#include <cmath>

namespace FCT {

class Mat4 {
public:
    float m[16];

    Mat4() = default;
    
    static Mat4 Identity() {
        return {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    }

    static Mat4 Translate(float x, float y, float z) {
        Mat4 m = Identity();
        m.m[12] = x;
        m.m[13] = y;
        m.m[14] = z;
        return m;
    }

    static Mat4 RotateZ(float degrees) {
        float radians = degrees * 3.14159265f / 180.0f;
        float c = cosf(radians);
        float s = sinf(radians);
        
        return {
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };
    }

    static Mat4 Scale(float x, float y, float z) {
        return {
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        };
    }

    Mat4 operator*(const Mat4& rhs) const {
        Mat4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i*4+j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i*4+j] += m[i*4+k] * rhs.m[k*4+j];
                }
            }
        }
        return result;
    }
};

} // namespace FCT
