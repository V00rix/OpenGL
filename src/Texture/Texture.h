//
// Created by vlado on 31-Jul-18.
//

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H


#include <GL/glew.h>

class Texture {
    unsigned id;

    const enum Type {
        t_2d = GL_TEXTURE_2D,
        t_cube = GL_TEXTURE_CUBE_MAP
    } type;
public:
    explicit Texture(const char *filePath); // simple 2D texture
    explicit Texture(const char *const filePath[6]); // cube map
    void use() const;
};


#endif //OPENGL_TEXTURE_H
