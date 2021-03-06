//
// Created by vlado on 21-Jul-18.
//

#include "ElementBase.h"
#include <glm/gtx/rotate_vector.hpp>

using namespace elements;

static const glm::vec2 uv_init[] = { // NOLINT
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 1.f),
};

const glm::vec2 *ElementBase::uv = uv_init;

void ElementBase::initBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::color_offset);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::uv_offset);

    glBindVertexArray(0);
}

ElementBase::~ElementBase() {
    printf("\tdeleting %x\n", this);
    printf("\tVAO = %d, VBO = %d, EBO = %d (%x)\n", VAO, VBO, EBO, this);
    delete[] vertices;
    delete[] indices;

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void ElementBase::render() const {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, 4);
}

void ElementBase::rotate(float angle, const glm::vec3 &direction) {
    TRANSL(for (int i = 0; i < vertexCount; i++) {
        vertices[i].position = glm::rotate(vertices[i].position, angle, direction);
        vertices[i].normal = glm::rotate(vertices[i].normal, angle, direction);
    })

    applyVertexTransform();
}

void ElementBase::translate(const glm::vec3 &to) {
    translation += to;

    for (unsigned i = 0; i < vertexCount; i++) {
        vertices[i].position += to;
    }

    applyVertexTransform();
}

void ElementBase::scale(const glm::vec3 &scale) {
    TRANSL(for (int i = 0; i < vertexCount; i++) {
        vertices[i].position *= scale;
    })

    applyVertexTransform();
}

void ElementBase::applyVertexTransform() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_DYNAMIC_DRAW);
}

void printPosition(const glm::vec3 &pos) {
    printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
}

void ElementBase::setPosition(const glm::vec3 &position) {
    printPosition(position);
    for (int i = 0; i < vertexCount; i++) {
        vertices[i].position -= translation;
        vertices[i].position += position;
    }

    translation = position;
    applyVertexTransform();

//    exit(0);
}
