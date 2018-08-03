//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "../../objects/ElementBase/ElementBase.h"
#include "../../GLModel/Mesh/Mesh.h"

class Cube : public Mesh {
protected:
    const float edge_length;
public:
    explicit Cube(float edge_length);

    Cube() : edge_length(0) {}
};

#endif //OPENGL_CUBE_H
