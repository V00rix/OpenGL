//
// Created by vlado on 20-Jul-18.
//

#include "Cube.h"
#include <iostream>

elements::Cube::Cube(float edge_length) : edge_length(edge_length) {
    initVertices();
    initBuffers();
}

void elements::Cube::initVertices() {
    glm::vec3 positions[8] = {
            // bottom square
            (glm::vec3(0.f, 0.f, 0.f)),                                              // 0
            (glm::vec3(edge_length, 0.f, 0.f)),                                // 1
            (glm::vec3(edge_length, 0.f, edge_length)),                  // 2
            (glm::vec3(0.f, 0.f, edge_length)),                                // 3

            // top square
            (glm::vec3(0.f, edge_length, 0.f)),                                // 4
            (glm::vec3(edge_length, edge_length, 0.f)),                  // 5
            (glm::vec3(edge_length, edge_length, edge_length)),    // 6
            (glm::vec3(0.f, edge_length, edge_length)),                  // 7
    };

    glm::vec3 normals[6]{
            glm::vec3(.0f, -1.f, .0f),  // bottom
            glm::vec3(.0f, 1.f, .0f),   // top
            glm::vec3(0.f, 0.f, 1.f),   // front
            glm::vec3(0.f, 0.f, -1.f),  // back
            glm::vec3(-1.f, 0.f, 0.f),  // left
            glm::vec3(1.f, 0.f, 0.f),   // right

    };

    vertices = new util::Vertex[vertexCount]{
            // bottom square
            util::Vertex(positions[0], normals[0], uv[3]),     // 0
            util::Vertex(positions[1], normals[0], uv[2]),     // 1
            util::Vertex(positions[2], normals[0], uv[1]),     // 2
            util::Vertex(positions[3], normals[0], uv[0]),     // 3
            // top square
            util::Vertex(positions[4], normals[1], uv[0]),     // 4
            util::Vertex(positions[5], normals[1], uv[1]),     // 5
            util::Vertex(positions[6], normals[1], uv[2]),     // 6
            util::Vertex(positions[7], normals[1], uv[3]),     // 7
            // front square
            util::Vertex(positions[0], normals[2], uv[0]),     // 8
            util::Vertex(positions[1], normals[2], uv[1]),     // 9
            util::Vertex(positions[5], normals[2], uv[2]),     // 10
            util::Vertex(positions[4], normals[2], uv[3]),     // 11
            // back square
            util::Vertex(positions[2], normals[3], uv[0]),     // 12
            util::Vertex(positions[6], normals[3], uv[3]),     // 13
            util::Vertex(positions[7], normals[3], uv[2]),     // 14
            util::Vertex(positions[3], normals[3], uv[1]),     // 15
            // left square
            util::Vertex(positions[0], normals[4], uv[1]),     // 16
            util::Vertex(positions[4], normals[4], uv[2]),     // 17
            util::Vertex(positions[7], normals[4], uv[3]),     // 18
            util::Vertex(positions[3], normals[4], uv[0]),     // 19
            // right square
            util::Vertex(positions[1], normals[5], uv[0]),     // 20
            util::Vertex(positions[5], normals[5], uv[3]),     // 21
            util::Vertex(positions[6], normals[5], uv[2]),     // 22
            util::Vertex(positions[2], normals[5], uv[1]),     // 23
    };

    indices = new unsigned[indexCount]{
            // bottom
            0, 1, 2,
            0, 2, 3,

            // top
            4, 6, 5,
            4, 7, 6,

            // right
            20, 21, 22,
            20, 22, 23,

            // left
            16, 18, 17,
            16, 19, 18,

            // back
            8, 10, 9,
            8, 11, 10,

            // front
            12, 13, 14,
            12, 14, 15,
    };

    ElementBase::indexCount = indexCount;
    ElementBase::indexSize = indexSize;
    ElementBase::vertexSize = vertexSize;
    ElementBase::vertexCount = vertexCount;
}
