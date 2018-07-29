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
    memcpy((void *)ElementBase::indices, &indices[0], indexSize);
}

elements::Mesh::Mesh(const elements::Mesh &another) : ElementBase(another), objFilePath(another.objFilePath),
                                                      at(another.at) {
    initBuffers();
}

void elements::Mesh::copy(elements::Mesh mesh) {

}

elements::Mesh &elements::Mesh::operator=(elements::Mesh other) {
    printf("copy assignment other(%x) to this(%x)\n", &other, this);

//    this->vertices = new

//    VBO = other.VBO;


    vertexSize = other.vertexSize;
    vertices = other.vertices;
    printf("vertices other(%x) vs this(%x)\n", this->vertices, other.vertices);
    return *this;
}
