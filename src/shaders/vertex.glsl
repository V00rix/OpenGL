#version 330 core

layout(location = 0) in vec3 position;

uniform vec3 u_scale = vec3(.5, .5, .5);

void main() {
    gl_Position = vec4(u_scale.x * position.x, u_scale.y * position.y, u_scale.z * position.z, 1.0);
}