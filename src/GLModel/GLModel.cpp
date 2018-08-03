//
// Created by vlado on 01-Aug-18.
//

#include "GLModel.h"

void GLModel::render() const {
    // use texture
    texture.use();

    // set material uniforms
    material.use();

    // render mesh
    mesh.render();
}

void GLModel::cleanGL() const {
    mesh.cleanGL();
    texture.cleanGL();
    material.cleanGL();
}

void GLModel::printData() const {

}
