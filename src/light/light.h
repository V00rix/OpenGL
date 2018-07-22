//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H


#include <glm/vec3.hpp>
#include <GL/glew.h>

namespace light {
    struct BaseLight {
        glm::vec3 color;
        float ambientIntensity;
        float diffuseIntensity;
    };

    struct Directional : BaseLight {
        glm::vec3 direction;

        Directional(const BaseLight& b, const glm::vec3 & d): BaseLight(b), direction(d) {}
    };

    struct Point : BaseLight {
        glm::vec3 position;

        struct {
            float constant;
            float linear;
            float exponential;
        } attenuation;
    };

    struct Spot : BaseLight {

    };

    void SetPointLights(unsigned int count, const Point* lights);
}
#endif //OPENGL_LIGHT_H
