//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_UTIL_H
#define OPENGL_UTIL_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define FOURCC_DXT1 827611204 // 'D' 'X' 'T' '1'
#define FOURCC_DXT3 861165636 // 'D' 'X' 'T' '3'
#define FOURCC_DXT5 894720068 // 'D' 'X' 'T' '5'

namespace util {

    /**
     * Load DDS texture into memory
     * @param imagePath
     * @return
     */
    unsigned loadDDS(const char *imagePath);

    unsigned loadBMP(const char *imagePath);


    struct Vertex {
        explicit Vertex(const glm::vec3 &pos) : m_pos(pos), m_col(0.f), m_uv(0.f) {
        }

        Vertex(const glm::vec3 &pos, const glm::vec2 &uv) : Vertex(pos) {
            m_uv = uv;
        }

        static const unsigned size = sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);
        static const unsigned color_offset = sizeof(glm::vec3);
        static const unsigned uv_offset = sizeof(glm::vec3) + sizeof(glm::vec3);

        glm::vec3 m_pos;
        glm::vec3 m_col;
        glm::vec2 m_uv;

        Vertex() = default;

        Vertex(const glm::vec3 pos, const glm::vec3 col, const glm::vec2 tex) : m_pos(pos), m_col(col), m_uv(tex) {}
    };

}
#endif //OPENGL_UTIL_H
