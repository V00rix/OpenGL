#version 400 core

const int MAX_POINT_LIGHTS = 2;

in vec2 UV;
in vec3 model_normal;
in vec3 model_position;

out vec3 color;

struct Attenuation {
        float constant;
        float linear;
        float exponential;
    };

struct BaseLight {
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct Directional {
    BaseLight base;

    vec3 direction;
};

struct Point {
    BaseLight base;
    Attenuation attenuation;
    vec3 position;

};

uniform int point_lights_count;
uniform Directional directional_light;

//uniform vec4 directional_ambient;
//uniform vec4 directional_diffuse;
uniform sampler2D texture_sampler;

uniform vec3 camera_position;
uniform float specular_intensity;
uniform float specular_power;

void main(){
    vec3 ambient_color = vec3(directional_light.base.color * directional_light.base.ambient_intensity);

    vec3 light_direction = -directional_light.direction;
    vec3 normal = normalize(model_normal);

    float diffuse_factor = dot(normal, light_direction);

    vec3 diffuse_color = vec3(0, 0, 0);
    vec3 specular_color = vec3(0, 0, 0);

    if (diffuse_factor > 0) {
        diffuse_color = vec3(directional_light.base.color * directional_light.base.diffuse_intensity * diffuse_factor);

        vec3 vertex_to_eye = normalize(camera_position - model_position);
        vec3 light_reflect = normalize(reflect(directional_light.direction, normal));
        float specular_factor = dot(vertex_to_eye, light_reflect);

        if (specular_factor > 0) {
            specular_factor = pow(specular_factor, specular_power);
            specular_color = directional_light.base.color * specular_intensity * specular_factor;
        }
    }

     color = texture2D(texture_sampler, UV.xy).rgb * (ambient_color + diffuse_color + specular_color);
}