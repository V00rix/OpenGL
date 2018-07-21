#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 model_normal; // normal relative to world transformations
out vec3 model_position; // position relative to world transformations

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    mat4 MVP = projection * view * model;

    gl_Position = MVP * vec4(position, 1);

    model_normal = (model * vec4(normal, 0.0)).xyz;
    model_position = (model * vec4(position, 0.0)).xyz;

    UV = vertexUV;
}