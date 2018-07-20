#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentColor;

out vec2 UV;

uniform mat4 MVP;
uniform mat4 u_scale;
uniform mat4 u_translate;

void main() {
    gl_Position = MVP * u_translate * u_scale * vec4(position,1);
    UV = vertexUV;
    fragmentColor = in_color;
}