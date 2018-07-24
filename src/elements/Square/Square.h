//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_SQUARE_H
#define OPENGL_SQUARE_H

#include "../ElementBase/ElementBase.h"

namespace elements {

    class Square : public ElementBase {

    private:
        static const unsigned vertexCount = 4 * 6;    // 4 vertices per square
        static const unsigned vertexSize = vertexCount * util::Vertex::size;
        static const unsigned indexCount = 3 * 2 * 6;  // 3 indices per triangle
        static const unsigned indexSize = indexCount * sizeof(unsigned);

        const glm::vec2 *uv;

        const glm::vec3 at;
        const float edge_length;

        void initVertices() override;

    public:
        Square(const glm::vec3 &, float);

        Square(const glm::vec3 &, float, const glm::vec2*);
    };

}

#endif //OPENGL_SQUARE_H
