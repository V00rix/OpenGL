//
// Created by vlado on 26-Jul-18.
//

#include "Mesh.h"

elements::Mesh::Mesh(const char *objFilePath, const glm::vec3 &at) : objFilePath(objFilePath), at(at) {
    initVertices();
    initBuffers();
}

void elements::Mesh::initVertices() {
    std::vector<util::Vertex> vertices;
    std::vector<unsigned> indices;
    util::loadOBJ(objFilePath, vertices, indices);

    indexCount = indices.size();
    indexSize = indexCount * sizeof(unsigned);
    vertexCount = vertices.size();
    vertexSize = vertexCount * util::Vertex::size;

    ElementBase::vertices = new util::Vertex[vertexCount];
    ElementBase::indices = new unsigned[indexCount];

    memcpy(ElementBase::vertices, &vertices[0], vertexSize);
    memcpy((void *) ElementBase::indices, &indices[0], indexSize);
}

void elements::Mesh::set(const elements::Mesh &other) {
    vertexCount = other.vertexCount;
    vertexSize = other.vertexSize;

    indexCount = other.indexCount;
    indexSize = other.indexSize;


    vertices = new util::Vertex[vertexCount];
    indices = new unsigned[indexCount];

    memcpy(vertices, other.vertices, vertexSize);
    memcpy(indices, other.indices, indexSize);

    initBuffers();
}
