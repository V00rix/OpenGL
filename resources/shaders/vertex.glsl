#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertexUV;

out VS_OUT {
    vec2 UV;
    vec3 model_normal;
    vec3 model_position;
} vs_out;

layout (std140) uniform Matrices {
    uniform mat4 projection;
    uniform mat4 view;
};

uniform mat4 world;
uniform bool draw_ui;

void main() {
    mat4 MVP = projection * view * world;
    gl_Position = MVP * vec4(position, 1.f);
    vs_out.model_normal = (world * vec4(normal, 0.0)).xyz;
    vs_out.model_position = (world * vec4(position, 0.0)).xyz;
//    gl_PointSize = gl_Position.z / 2.f;

    vs_out.UV = vertexUV;
}