//
// Created by vlado on 01-Aug-18.
//

#include "Mesh.h"
#include "../../Program/Program.h"
#include "../../util/uniforms.h"

Mesh::Mesh(const unsigned verticesCount,
           const unsigned indicesCount) : verticesCount(verticesCount),
                                          indicesCount(indicesCount),
                                          verticesSize(verticesCount * util::Vertex::size),
                                          indicesSize(indicesCount * sizeof(unsigned)) {}

Mesh::Mesh(util::Vertex *v, const unsigned *i,
           const unsigned verticesCount, const unsigned indicesCount) : Mesh(verticesCount, indicesCount) {
    vertices = v;
    indices = i,
            init();
}

void Mesh::render() const {
    // check
    if (!initialized) {
        printf("[Trying to render uninitialized mesh]\n");
    }

    // set model transformation matrix
    (*Program::active).setMat4(UNI_MODEL, model);

    // render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
}

void Mesh::cleanGL() const {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::init() {
    if (!vertices || !indices || !verticesCount || !verticesSize || !indicesCount || !indicesCount) {
        crashGL("[Could not init mesh]\n");
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::color_offset);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::uv_offset);

    glBindVertexArray(0);

    initialized = true;
}

void Mesh::printData() const {
    printf("Printing data for mesh (%x):\n", this);
    printf("\tVAO = %d\n", VAO);
    printf("\tVBO = %d\n", VBO);
    printf("\tEBO = %d\n", EBO);
    printf("\t----------\n");
    printf("\tvertices\t\t-> %x\n", vertices);
    printf("\tindices\t\t-> %x\n", indices);
    printf("\t----------\n");
    printf("\tverticesCount\t= %d\n", verticesCount);
    printf("\tindicesCount\t= %d\n", indicesCount);
    printf("\tindicesSize\t= %d\n", indicesSize);
    printf("\tverticesSize\t= %d\n", verticesSize);
}
