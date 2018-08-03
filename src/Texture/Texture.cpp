//
// Created by vlado on 31-Jul-18.
//

#include "Texture.h"
#include "../util/util.h"

Texture::Texture(const char *filePath) : type(t_2d) {
    id = loadTexture(filePath);
}

Texture::Texture(const char *const filePath[6]) : type(t_cube) {
    id = util::loadSkyBoxBMP(filePath);
}

void Texture::use() const {
    glBindTexture(type, id);
}

unsigned Texture::loadTexture(const char *filePath) {
    util::FExtension ext = util::getFileExtension(filePath);
    GLuint texture;
    switch (ext) {
        case util::bmp:
            texture = util::loadBMP(filePath);
            return texture;
        case util::dds:
            texture = util::loadDDS(filePath);
            return texture;
        default:
            printf("Unsupported texture format %s", filePath);
            return 0;
    }
}

void Texture::printData() const {

}
