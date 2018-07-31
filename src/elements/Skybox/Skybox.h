//
// Created by vlado on 31-Jul-18.
//

#ifndef OPENGL_SKYBOX_H
#define OPENGL_SKYBOX_H

#include "../../Texture/Texture.h"
#include "../Cube/Cube.h"

namespace elements {

    class Skybox : public Cube {
        Texture texture;

        void initVertices() override;
    public:
        explicit Skybox(const char *const filePath[6]);

        void render() const override;
    };
};

#endif //OPENGL_SKYBOX_H
