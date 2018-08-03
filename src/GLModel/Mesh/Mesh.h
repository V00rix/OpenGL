//
// Created by vlado on 01-Aug-18.
//

#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H


#include <glm/vec3.hpp>
#include <functional>
#include <glm/detail/type_mat4x4.hpp>
#include "../../GLObject/GLObject.h"
#include "../../util/util.h"

class Mesh : GLObject {
protected:
    // Vertex data
    util::Vertex *vertices{nullptr};
    unsigned verticesSize{0};
    unsigned verticesCount{0};

    // Index data
    const unsigned *indices{nullptr};
    unsigned indicesSize{0};
    unsigned indicesCount{0};

    // Render data
    unsigned VAO{0}, VBO{0}, EBO{0};

    // Model matrix
    glm::mat4 model{1.f};

    // Initialization status
    bool initialized{false};

    // Setup buffers
    void init();

    // Helper debug function
    void printData() const override;

public:
    Mesh() = default;

    Mesh(util::Vertex *v, const unsigned *i) : vertices{v}, indices{i} {}

    Mesh(util::Vertex *v, const unsigned *i, unsigned verticesCount, unsigned indicesCount);

    Mesh(unsigned verticesCount, unsigned indicesCount);

    void cleanGL() const override;

    void transform(const glm::mat4 &transformation) { this->model *= transformation; }

    void setModel(const glm::mat4 &model) { this->model *= model; }

    void render() const;
};


#endif //OPENGL_MESH_H
