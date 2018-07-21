#version 400 core

in vec2 UV;
in vec3 model_normal;
in vec3 model_position;

out vec3 color;

uniform vec4 directional_ambient;
uniform vec4 directional_diffuse;
uniform sampler2D texture_sampler;

uniform vec3 camera_position;
uniform float specular_intensity;
uniform float specular_power;

void main(){
    vec3 ambient_color = vec3(directional_ambient.rgb * directional_ambient.a);

    vec3 light_direction = -directional_diffuse.xyz;
    vec3 normal = normalize(model_normal);

    float diffuse_factor = dot(normal, -directional_diffuse.xyz);

    vec3 diffuse_color = vec3(0, 0, 0);
    vec3 specular_color = vec3(0, 0, 0);

    if (diffuse_factor > 0) {
        diffuse_color = vec3(directional_ambient.rgb * directional_diffuse.a * diffuse_factor);

        vec3 vertex_to_eye = normalize(camera_position - model_position);
        vec3 light_reflect = normalize(reflect(directional_diffuse.xyz, normal));
        float specular_factor = dot(vertex_to_eye, light_reflect);

        if (specular_factor > 0) {
            specular_factor = pow(specular_factor, specular_power);
            specular_color = directional_ambient.rgb * specular_intensity * specular_factor;
        }
    }

     color = texture2D(texture_sampler, UV.xy).rgb * (ambient_color + diffuse_color + specular_color);
}