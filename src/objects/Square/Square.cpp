//
// Created by vlado on 21-Jul-18.
//

#include "Square.h"

void elements::Square::initVertices() {
    glm::vec3 positions[4] = {
            (glm::vec3(0.f, 0.f, 0.f)),                                              // 0
            (glm::vec3(edge_length, 0.f, 0.f)),                                // 1
            (glm::vec3(edge_length, edge_length, 0.f)),                  // 2
            (glm::vec3(0.f, edge_length, 0.f)),                                // 3
    };


    vertices = new util::Vertex[vertexCount]{
            util::Vertex(positions[0], glm::vec3(0.f, 0.0f, 1.f), this->uv[0]),     // 0
            util::Vertex(positions[1], glm::vec3(0.f, 0.0f, 1.f), this->uv[1]),     // 1
            util::Vertex(positions[2], glm::vec3(0.f, 0.0f, 1.f), this->uv[2]),     // 2
            util::Vertex(positions[3], glm::vec3(0.f, 0.0f, 1.f), this->uv[3]),     // 3
    };

    indices = new unsigned[indexCount]{
            0, 1, 2,
            0, 2, 3,
    };

    ElementBase::indexCount = Square::indexCount;
    ElementBase::indexSize = Square::indexSize;
    ElementBase::vertexSize = Square::vertexSize;
    ElementBase::vertexCount = vertexCount;
}

elements::Square::Square(float edge_length) : edge_length(edge_length),
                                              uv(ElementBase::uv) {
    initVertices();
    initBuffers();
}

elements::Square::Square(float edge_length, const glm::vec2 *uv) :
        edge_length(edge_length),
        uv(uv) {
    initVertices();
    initBuffers();
}
