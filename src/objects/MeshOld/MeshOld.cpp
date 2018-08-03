//
// Created by vlado on 26-Jul-18.
//

#include "MeshOld.h"

elements::MeshOld::MeshOld(const char *objFilePath) : objFilePath(objFilePath) {
    initVertices();
    initBuffers();
}

void elements::MeshOld::initVertices() {
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

void elements::MeshOld::set(const elements::MeshOld &other) {
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
