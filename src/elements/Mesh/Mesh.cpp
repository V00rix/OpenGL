//
// Created by vlado on 26-Jul-18.
//

#include "Mesh.h"

elements::Mesh::Mesh(const char *objFilePath, const glm::vec3 &at) : objFilePath(objFilePath), at(at) {
    initVertices();
    initBuffers();
}

void elements::Mesh::initVertices() {
    auto vertices = new std::vector<util::Vertex>;
    auto ind = new std::vector<unsigned>;
    util::loadOBJ(objFilePath, *vertices, *ind);

    indexCount = ind->size();
    indexSize = indexCount * sizeof(unsigned);
    vertexCount = vertices->size();
    vertexSize = vertexCount * util::Vertex::size;

    ElementBase::vertices = &(*vertices)[0];
    ElementBase::indices = &(*ind)[0];

}