#version 330 core

layout(location = 0) in vec3 position;

void main() {
    gl_Position = vec4(.5 * position.x, .5 * position.y, .5*position.z, 1.0);
}