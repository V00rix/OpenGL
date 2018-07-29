//
// Created by vlado on 26-Jul-18.
//

#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include "../ElementBase/ElementBase.h"

namespace elements {

    class Mesh : public ElementBase {
        const char *objFilePath;

        glm::vec3 at;

        void initVertices() override;

    public:
        Mesh(const char *objFilePath, const glm::vec3 &at);

        Mesh(const Mesh &another);

        Mesh() : ElementBase(), objFilePath(nullptr), at(glm::vec3(0.f)) {}

        void copy(Mesh mesh);

        Mesh& operator=(Mesh other);
    };
}

#endif //OPENGL_MESH_H
