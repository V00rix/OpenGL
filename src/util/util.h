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
        glm::vec3 m_pos;
        glm::vec3 m_col;
        glm::vec2 m_tex;

        Vertex() = default;

        Vertex(glm::vec3 pos, glm::vec3 col, glm::vec2 tex) : m_pos(pos), m_col(col), m_tex(tex) {}
    };

}
#endif //OPENGL_UTIL_H
