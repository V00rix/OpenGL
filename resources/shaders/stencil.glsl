#version 400 core

out vec4 color;

const int MAX_POINT_LIGHTS = 2;
const int MAX_DIRECTIONAL_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;


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

struct Spot {
    Point point;
    vec3 direction;
    float cutoff;
};

uniform int point_lights_count;
uniform Point[MAX_POINT_LIGHTS] point_lights;
uniform int point_index;

uniform int directional_lights_count;
uniform Directional[MAX_DIRECTIONAL_LIGHTS] directional_lights;
uniform int directional_index;


uniform int spot_lights_count;
uniform Spot[MAX_SPOT_LIGHTS] spot_lights;
uniform int spot_index;

uniform sampler2D texture_sampler;

uniform vec3 camera_position;


uniform float specular_intensity;
uniform float specular_power;

uniform bool grid_enabled;

uniform bool light_mesh;

void main(){
    color = vec4(.04f, .28f, .26f, 1.f);
}
