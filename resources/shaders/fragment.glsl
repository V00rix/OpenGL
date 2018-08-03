#version 400 core
//
//const int MAX_POINT_LIGHTS = 2;
//const int MAX_DIRECTIONAL_LIGHTS = 2;
//const int MAX_SPOT_LIGHTS = 2;
//
//struct Attenuation {
//    float constant, linear, exponential;
//};
//
//struct BaseLight {
//    vec3 ambient, diffuse, specular;
//};
//
//struct Directional {
//    BaseLight base;
//    vec3 direction;
//};
//
//struct Point {
//    BaseLight base;
//    Attenuation attenuation;
//    vec3 position;
//};
//
//struct Spot {
//    Point point;
//    vec3 direction;
//    float cutoff;
//};
//
//uniform struct Material {
//    vec3 ambient, diffuse, specular;
//    float shininess;
//} material;
//
//// Input
//in GS_OUT {
//    vec2 UV;
//    vec3 model_normal; // normal relative to world transformations
//    vec3 model_position; // position relative to world transformations
//} fs_in;
//
//// Uniforms
//uniform int directional_lights_count;
//uniform Directional[MAX_DIRECTIONAL_LIGHTS] directional_lights;
//uniform int directional_index;
//
//uniform int point_lights_count;
//uniform Point[MAX_POINT_LIGHTS] point_lights;
//uniform int point_index;
//
//uniform int spot_lights_count;
//uniform Spot[MAX_SPOT_LIGHTS] spot_lights;
//uniform int spot_index;
//
//
//uniform sampler2D texture_sampler;
//uniform vec3 camera_position;
//
//uniform bool grid_enabled;
//uniform bool light_mesh;
//
// Output
out vec4 FragColor;
//
//vec3 calculateDirectionalLight(BaseLight light, vec3 direction, vec3 normal) {
//    // calculate ambient property
//    vec3 ambientColor = vec3(light.ambient * light.ambient_intensity);
//
//    float diffuseFactor = dot(normal, -direction);
//
//    vec3 diffuseColor, specularColor = diffuseColor = vec3(0f);
//
//    if (diffuseFactor > 0) {
//        // calculate diffuse
//        diffuseColor = light.color * light.diffuse_intensity * diffuseFactor;
//
//        // calculate specular
//        vec3 vertextToEye = normalize(camera_position - fs_in.model_position);
//        vec3 lightReflect = normalize(reflect(direction, normal));
//        float specularFactor = dot(vertextToEye, lightReflect);
//
//        if (specularFactor > 0) {
//            specularFactor = pow(specularFactor, specular_power);
//            specularColor = light.color * specular_intensity * specularFactor;
//        }
//    }
//
//    return (ambientColor + diffuseColor + specularColor);
//}
//
//vec3 calculatePointLight(Point light, vec3 normal) {
//    vec3 direction = fs_in.model_position - light.position;
//    float distance = length(direction);
//
//    direction = normalize(direction);
//    float attenuation = light.attenuation.constant
//                     + light.attenuation.linear * distance
//                     + light.attenuation.exponential * distance * distance;
//
//    return calculateDirectionalLight(light.base, direction, normal) / attenuation;
//}
//
//vec3 calculateSpotLight(Spot light, vec3 normal) {
//    vec3 lightToPixel = normalize(fs_in.model_position - light.point.position);
//    float spotFactor = dot(lightToPixel, light.direction);
//
//    if (spotFactor > light.cutoff) {
//        return calculatePointLight(light.point, normal)* (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - light.cutoff));
//    }
//
//    return vec3(0);
//}

void main(){
//    if (grid_enabled) {
//        FragColor = vec4(.3f);
//
//    } else if (light_mesh) {
//       FragColor = vec4(point_lights[point_index].base.color, 1.f);
//    } else {
//        vec3 normal = normalize(fs_in.model_normal);
//
//        vec3 totalLight = vec3(0);
//
//        int count;
//        count = min(directional_lights_count, MAX_DIRECTIONAL_LIGHTS);
//
//        for (int i = 0; i < count; i++) {
//            totalLight += calculateDirectionalLight(directional_lights[i].base, directional_lights[i].direction, normal);
//        }
//
//        count = min(point_lights_count, MAX_POINT_LIGHTS);
//
//        for (int i = 0; i < count; i++) {
//            totalLight += calculatePointLight(point_lights[i], normal);
//        }
//
//        count = min(spot_lights_count, MAX_SPOT_LIGHTS);
//
//        for (int i = 0; i < count; i++) {
//            totalLight += calculateSpotLight(spot_lights[i], normal);
//        }
//
//        FragColor = texture2D(texture_sampler, fs_in.UV.xy) * vec4(totalLight, 1.f);
//    }
    FragColor = vec4(1.f);
}
