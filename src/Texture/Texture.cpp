//
// Created by vlado on 31-Jul-18.
//

#include "Texture.h"

Texture::Texture(const char *filePath): type(t_2d) {

}

Texture::Texture(const char *const filePath[6]): type(t_cube) {

}

void Texture::use() const {
    glBindTexture(type, id);
}
