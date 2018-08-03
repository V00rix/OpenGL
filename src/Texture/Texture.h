//
// Created by vlado on 31-Jul-18.
//

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H


#include <GL/glew.h>
#include <string>
#include "../GLObject/GLObject.h"

class Texture : GLObject {
    unsigned id{0};

    enum Type {
        t_2d = GL_TEXTURE_2D,
        t_cube = GL_TEXTURE_CUBE_MAP
    } type{t_2d};

    unsigned loadTexture(const char *);

    void printData() const override;
public:
    Texture() = default;

    explicit Texture(const char *filePath); // simple 2D texture
    explicit Texture(const char *const filePath[6]); // cube map
    void use() const;

    inline void cleanGL() const override {}

    explicit operator unsigned() const { return id; };
};

#endif //OPENGL_TEXTURE_H
