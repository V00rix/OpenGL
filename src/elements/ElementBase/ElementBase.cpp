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
    glBindVertexArray(VAO);

    printf("first: %d\n", VBO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_DYNAMIC_DRAW);

    printf("second: %d\n", VBO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::color_offset);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::uv_offset);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ElementBase::~ElementBase() {
    printf("destructing %x\n", this);

    delete[] vertices;
    delete[] indices;

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
}

void ElementBase::render() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void ElementBase::rotate(float angle, const glm::vec3 &direction) {
    TRANSL(for (int i = 0; i < vertexCount; i++) {
        vertices[i].position = glm::rotate(vertices[i].position, angle, direction);
    })

    applyVertexTransform();
}

void ElementBase::translate(const glm::vec3 &to) {
    translation += to;

    for (int i = 0; i < vertexCount; i++) {
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
    printf("TRANSFORM VBO: %d\n", VBO);
    printf("TRANSFORM vertexSize: %x\n", vertexSize);
    printf("TRANSFORM vertices: %x\n", &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_DYNAMIC_DRAW);
}

void ElementBase::setPosition(const glm::vec3 &position) {
    printf("try and get me...\n");
    printf("on (%x) x = %f", &vertices[0], vertices[0].position.x);
    for (int i = 0; i < vertexCount; i++) {
        vertices[i].position -= translation;
        vertices[i].position += position;
    }

    translation = glm::vec3(0.f);
    applyVertexTransform();
}

ElementBase::ElementBase(const ElementBase &another) : indexCount(another.indexCount),
                                                       indexSize(another.indexSize),
                                                       vertexCount(another.vertexCount),
                                                       vertexSize(another.vertexSize),
                                                       vertices(another.vertices),
                                                       indices(another.indices) {}

ElementBase::ElementBase() : indexCount(0),
                             indexSize(0),
                             vertexCount(0),
                             vertexSize(0),
                             vertices(nullptr),
                             indices(nullptr) {}
