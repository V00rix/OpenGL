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
    printf("init bufers\n");
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    printf("VAO = %d (%x)\n", VAO, this);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_DYNAMIC_DRAW);

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


    printf("VAO = %d, VBO = %d, IBO = %d (%x)\n", VAO, VBO, IBO, this);
}

ElementBase::~ElementBase() {
    printf("\tdeleting %x\n", this);
    printf("\tVAO = %d, VBO = %d, IBO = %d (%x)\n", VAO, VBO, IBO, this);
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_DYNAMIC_DRAW);
}

void printPosition(const glm::vec3 &pos) {
    printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
}

void ElementBase::setPosition(const glm::vec3 &position) {
    printf("\t\tposition = ");
    printPosition(position);
    for (int i = 0; i < vertexCount; i++) {
//        printf("before: ");
//        printPosition(vertices[i].position);
        vertices[i].position -= translation;
        vertices[i].position += position;
//        printf("after: ");
//        printPosition(vertices[i].position);
    }

    translation = position;
    applyVertexTransform();

//    exit(0);
}
