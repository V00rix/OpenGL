//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H


#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cstring>

namespace light {
    struct Base {
        glm::vec3 color;
        float ambientIntensity;
        float diffuseIntensity;
    };

    struct Directional : Base {
        glm::vec3 direction;

        Directional(const Base &base, const glm::vec3 &direction) : Base(base), direction(direction) {}
    };

    struct Point : Base {
        glm::vec3 position;

        struct Attenuation {
            float constant;
            float linear;
            float exponential;
        } attenuation;

        Point(const Base &base, const glm::vec3 &position, const Attenuation &attenuation) : Base(base),
                                                                                             position(position),
                                                                                             attenuation(attenuation) {}
    };

    struct Spot : Base {

    };

    namespace uni {
        struct u_base {
            int color;
            int ambient_intensity;
            int diffuse_intensity;
        };

        struct u_directional {
            u_base base;

            int u_direction;
        };

        struct u_point {
            u_base base;

            struct Attenuation {
                int constant;
                int linear;
                int exponential;
            } attenuation;

            int position;
        };

        u_directional getDirectional(GLuint, const char *);
        u_directional getDirectional(GLuint, std::string);
        void setDirectional(const u_directional &, const light::Directional &);

        u_point getPoint(GLuint, const char *);

        void setPoint(const u_point &, const light::Point &);
    }

}
#endif //OPENGL_LIGHT_H
