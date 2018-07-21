//
// Created by vlado on 21-Jul-18.
//

#include "Square.h"

void elements::Square::draw() const {

}

void elements::Square::initVertices(util::Vertex *vertices, unsigned *indices,
                                    unsigned *vertexSize, unsigned *indexSize) {
    glm::vec3 positions[4] = {
            (glm::vec3(at.x, at.y, at.z)),                                              // 0
            (glm::vec3(at.x + edge_length, at.y, at.z)),                                // 1
            (glm::vec3(at.x + edge_length, at.y + edge_length, at.z)),                  // 2
            (glm::vec3(at.x, at.y + edge_length, at.z)),                                // 3
    };

    vertices = new util::Vertex[vertexCount]{
            util::Vertex(positions[0], uv[3]),     // 0
            util::Vertex(positions[1], uv[2]),     // 1
            util::Vertex(positions[2], uv[1]),     // 2
            util::Vertex(positions[3], uv[0]),     // 3
    };

    indices = new unsigned[indexCount]{
            0, 1, 2,
            0, 2, 3,
    };

    *indexSize = Square::indexSize;
    *vertexSize = Square::vertexSize;
}

elements::Square::Square()
        : ElementBase(), edge_length(0.f) {}
