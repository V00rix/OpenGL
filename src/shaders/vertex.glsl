#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 MVP;
uniform mat4 u_scale;
uniform mat4 u_translate;

void main() {
    gl_Position = MVP * u_scale * u_translate * vec4(position,1);
    fragmentColor = vertexColor;
}