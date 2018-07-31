#version 400 core

layout(location = 0) in vec3 position;

out vec3 textureCoords;

layout (std140) uniform Matrices {
    uniform mat4 projection;
    uniform mat4 view;
};

void main() {
    textureCoords = position;
    vec4 pos = projection * view * vec4(position, 1.f);
    gl_Position = pos.xyww;
}