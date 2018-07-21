//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_SQUARE_H
#define OPENGL_SQUARE_H

#include "ElementBase.h"

namespace elements {

    class Square : ElementBase {

    private:
        static const unsigned vertexCount = 4 * 6;    // 4 vertices per square
        static const unsigned vertexSize = vertexCount * util::Vertex::size;
        static const unsigned indexCount = 3 * 2 * 6;  // 3 indices per triangle
        static const unsigned indexSize = indexCount * sizeof(unsigned);

        const glm::vec3 at;
        const float edge_length;

        void initVertices(util::Vertex *vertices, unsigned *indices, unsigned *vertexSize, unsigned *indexSize);

    public:
//        Square(const glm::vec3 &at, float edge_length) : ElementBase(initVertices), at(at), edge_length(edge_length) {}

        Square();

        void draw() const override;
    };

}

#endif //OPENGL_SQUARE_H
