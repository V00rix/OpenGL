//
// Created by vlado on 26-Jul-18.
//

#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include "../ElementBase/ElementBase.h"

namespace elements {

    class Mesh : public ElementBase {
        const char *objFilePath;

        void initVertices() override;

    public:

        explicit Mesh(const char *objFilePath);

        Mesh() : ElementBase(), objFilePath(nullptr) {}

        void set(const Mesh &other);
    };
}

#endif //OPENGL_MESH_H
