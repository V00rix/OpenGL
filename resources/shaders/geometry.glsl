#version 400 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_OUT {
    vec2 UV;
    vec3 model_normal; // normal relative to world transformations
    vec3 model_position; // position relative to world transformations
} gs_in[];

out GS_OUT {
    vec2 UV;
    vec3 model_normal; // normal relative to world transformations
    vec3 model_position; // position relative to world transformations
} gs_out;

uniform float time;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

void main() {
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.UV = gs_in[0].UV;
    gs_out.model_normal = gs_in[0].model_normal;
    gs_out.model_position = gs_in[0].model_position;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.UV = gs_in[1].UV;
    gs_out.model_normal = gs_in[1].model_normal;
    gs_out.model_position = gs_in[1].model_position;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.UV = gs_in[2].UV;
    gs_out.model_normal = gs_in[2].model_normal;
    gs_out.model_position = gs_in[2].model_position;
    EmitVertex();

//    gl_Position = gl_in[0].gl_Position + vec4(1.f);
//    EmitVertex();
//
//    gl_Position = gl_in[1].gl_Position + vec4(1.f);
//    EmitVertex();
//
//    gl_Position = gl_in[2].gl_Position + vec4(1.f);
//    EmitVertex();

    EndPrimitive();
}