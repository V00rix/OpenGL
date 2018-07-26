//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "../ElementBase/ElementBase.h"

namespace elements {

    class Cube : public ElementBase {
        static const unsigned vertexCount = 4 * 6;    // 4 vertices per square
        static const unsigned vertexSize = vertexCount * util::Vertex::size;
        static const unsigned indexCount = 3 * 2 * 6;  // 3 indices per triangle
        static const unsigned indexSize = indexCount * sizeof(unsigned);

        const glm::vec3 at;
        const float edge_length;

        void initVertices() override;

    public:
        Cube(const glm::vec3 &at, float edge_length);
    };
}


#endif //OPENGL_CUBE_H
