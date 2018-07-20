//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "../util/util.h"

namespace elements {

    class Cube {
    private:
        static const unsigned vertexCount = 4 * 6;    // 4 vertices per square
        static const unsigned vertexSize = vertexCount * util::Vertex::size;
        static const unsigned indexCount = 3 * 2 * 6;  // 3 indices per triangle
        static const unsigned indexSize = indexCount * sizeof(unsigned);

        util::Vertex *vertices;
        const unsigned *indices;

        mutable unsigned VAO;
        mutable unsigned VBO;
        mutable unsigned IBO;

    public:
        Cube(const glm::vec3 &at, float edge_length);

        virtual ~Cube();

        void setColors(const glm::vec3 *colors);

        inline void setColors(const float *colors) {
            setColors((glm::vec3 *) colors);
        }

        void setColors(const glm::vec3 &color);

        void draw() const;
    };
}


#endif //OPENGL_CUBE_H
