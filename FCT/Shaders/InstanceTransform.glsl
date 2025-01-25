#version 330 core

// 实例化矩阵属性（每实例数据）
layout(location = 5) in mat4 instanceTransform;

void applyInstanceTransform(inout vec4 position) {
    position = instanceTransform * position;
}
