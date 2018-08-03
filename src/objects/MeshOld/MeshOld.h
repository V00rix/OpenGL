//
// Created by vlado on 26-Jul-18.
//

#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include "../ElementBase/ElementBase.h"

namespace elements {

    class MeshOld : public ElementBase {
        const char *objFilePath;

        void initVertices() override;

    public:

        explicit MeshOld(const char *objFilePath);

        MeshOld() : ElementBase(), objFilePath(nullptr) {}

        void set(const MeshOld &other);
    };
}

#endif //OPENGL_MESH_H
