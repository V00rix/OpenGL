//
// Created by vlado on 31-Jul-18.
//

#include "Skybox.h"

void elements::Skybox::initVertices() {
    glm::vec3 positions[8] = {
            // bottom square
            (glm::vec3(0.f, 0.f, 0.f)),                                              // 0
            (glm::vec3(2.f, 0.f, 0.f)),                                // 1
            (glm::vec3(2.f, 0.f, 2.f)),                  // 2
            (glm::vec3(0.f, 0.f, 2.f)),                                // 3

            // top square
            (glm::vec3(0.f, 2.f, 0.f)),                                // 4
            (glm::vec3(2.f, 2.f, 0.f)),                  // 5
            (glm::vec3(2.f, 2.f, 2.f)),    // 6
            (glm::vec3(0.f, 2.f, 2.f)),                  // 7
    };

    for (auto &position : positions) {
        position -= 1.f;
    }

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
            util::Vertex(positions[6], normals[1], uv[3]),     // 4
            util::Vertex(positions[7], normals[1], uv[0]),     // 5
            util::Vertex(positions[4], normals[1], uv[1]),     // 6
            util::Vertex(positions[5], normals[1], uv[2]),     // 7
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
            0, 2, 1,
            0, 3, 2,

            // top
            4, 5, 6,
            4, 6, 7,

            // right
            20, 22, 21,
            20, 23, 22,

            // left
            16, 17, 18,
            16, 18, 19,

            // back
            8, 9, 10,
            8, 10, 11,

            // front
            12, 14, 13,
            12, 15, 14,
    };

    ElementBase::indexCount = indexCount;
    ElementBase::indexSize = indexSize;
    ElementBase::vertexSize = vertexSize;
    ElementBase::vertexCount = vertexCount;
}

elements::Skybox::Skybox(const char *const *filePath) : texture(filePath) {
    initVertices();
    initBuffers();
}

void elements::Skybox::render() const {
    texture.use();
    ElementBase::render();
}