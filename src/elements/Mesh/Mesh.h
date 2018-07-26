//
// Created by vlado on 26-Jul-18.
//

#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include "../ElementBase/ElementBase.h"

namespace elements {

    class Mesh : public ElementBase {
        const char *objFilePath;

        const glm::vec3 at;

        void initVertices() override;

    public:
        explicit Mesh(const char *objFilePath, const glm::vec3 &at);

        void render() const override;
    };
}

#endif //OPENGL_MESH_H
