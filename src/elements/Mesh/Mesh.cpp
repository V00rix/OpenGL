//
// Created by vlado on 26-Jul-18.
//

#include "Mesh.h"

elements::Mesh::Mesh(const char *objFilePath, const glm::vec3 &at) : objFilePath(objFilePath), at(at) {
    initVertices();
    initBuffers();
}

static void printVertex(const util::Vertex &vertex) {
    std::cout << "\tPosition: " << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z
              << std::endl;
    std::cout << "\tNormal: " << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << std::endl;
    std::cout << "\tTexture: " << vertex.uv.x << ", " << vertex.uv.y << std::endl;
}

static void printInfo(util::Vertex *vert, const unsigned *ind, unsigned v_count, unsigned ind_count) {
    for (ust i = 0; i < v_count; i++) {
        printVertex(vert[i]);
    }

    for (ust i = 0; i < ind_count; i++) {
        std::cout << "Index: " << ind[i] << "\n";
        printVertex(vert[ind[i]]);
    }
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

    printInfo(ElementBase::vertices, ElementBase::indices, vertexCount, indexCount);

}

void elements::Mesh::render() const {
    printf("indexCount=%d\n", indexCount);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

}