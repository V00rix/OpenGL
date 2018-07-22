//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_UTIL_H
#define OPENGL_UTIL_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <iostream>
#include "../light/light.h"

#define FOURCC_DXT1 827611204 // 'D' 'X' 'T' '1'
#define FOURCC_DXT3 861165636 // 'D' 'X' 'T' '3'
#define FOURCC_DXT5 894720068 // 'D' 'X' 'T' '5'

namespace util {

    struct Vertex {
        explicit Vertex(const glm::vec3 &pos) : position(pos), normal(0.f), uv(0.f) {
        }

        Vertex(const glm::vec3 &pos, const glm::vec2 &uv) : Vertex(pos) {
            this->uv = uv;
        }

        static const unsigned size = sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);
        static const unsigned color_offset = sizeof(glm::vec3);
        static const unsigned uv_offset = sizeof(glm::vec3) + sizeof(glm::vec3);

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;

        Vertex() = default;

        Vertex(const glm::vec3 pos, const glm::vec3 normal, const glm::vec2 tex) : position(pos), normal(normal),
                                                                                   uv(tex) {}
    };

    unsigned loadDDS(const char *);

    unsigned loadBMP(const char *);

    bool loadOBJ(const char *,
                 std::vector<Vertex> &);
}
#endif //OPENGL_UTIL_H
