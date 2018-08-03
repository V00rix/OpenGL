//
// Created by vlado on 02-Aug-18.
//

#ifndef OPENGL_GRID_H
#define OPENGL_GRID_H


#include "../../GLModel/Mesh/Mesh.h"

class Grid : Mesh {

    /**
     * Highest distance between two grid lines
     */
    float size;

    /**
     * Lines count
     */
    unsigned precision;
public:
    Grid(float size, unsigned precision);
};


#endif //OPENGL_GRID_H
