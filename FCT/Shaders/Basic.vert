#version 330 core

// 顶点属性
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;

// 实例化矩阵属性（每实例数据）
layout(location = 5) in vec4 instanceRow0;
layout(location = 6) in vec4 instanceRow1;
layout(location = 7) in vec4 instanceRow2;
layout(location = 8) in vec4 instanceRow3;

out vec4 v_color;
out vec2 v_texCoord;

uniform mat4 u_viewProj; // 视图投影矩阵

void main()
{
    // 组装实例矩阵
    mat4 instanceTransform = mat4(
        instanceRow0,
        instanceRow1,
        instanceRow2,
        instanceRow3
    );

    // 应用变换：MVP * 实例矩阵 * 顶点位置
    gl_Position = u_viewProj * instanceTransform * vec4(a_position, 0.0, 1.0);
    
    // 传递其他属性
    v_color = a_color;
    v_texCoord = a_texCoord;
}
