//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H


#include <glm/vec3.hpp>
#include <GL/glew.h>

namespace light {

    namespace type {
        struct Type {
            float intensity;

            explicit Type(const float &intensity) : intensity(intensity) {}
        };

        struct Ambient : Type {
            explicit Ambient(const float &intensity) : Type(intensity) {}
        };

        struct Diffuse : Type {
            glm::vec3 direction;

            Diffuse(const glm::vec3 &direction, const float &intensity) : Type(intensity) {
                this->direction = direction;
            }
        };

        struct Specular : Type {
            explicit Specular(const float &intensity) : Type(intensity) {}

        };
    }

    struct Source {
        glm::vec3 color;

        type::Ambient ambient;
        type::Diffuse diffuse;
        type::Specular specular;

        explicit Source(const glm::vec3 &color, const type::Ambient &ambient, const type::Diffuse &diffuse,
                        const type::Specular &specular) : color(color), ambient(ambient), diffuse(diffuse),
                                                          specular(specular) {}
    };

//    struct Point : Source {
//        Point(const glm::vec3 &color, const float &intensity) : Source(color, intensity) {}

//    };

    struct Direction : Source {

        Direction(const glm::vec3 &color, const light::type::Ambient &ambient,
                  const light::type::Diffuse &diffuse, const light::type::Specular &specular) : Source(color,
                                                                                                       ambient,
                                                                                                       diffuse,
                                                                                                       specular) {}
    };

//    struct Spot : Source {
//        Spot(const glm::vec3 &color, const float &intensity) : Source(color, intensity) {}
//
//    };

    inline void renderAmbient(const int &u_ambient, const Source &source) {
        glUniform4f(u_ambient, source.color.r, source.color.g, source.color.b, source.ambient.intensity);
    }
}
#endif //OPENGL_LIGHT_H
