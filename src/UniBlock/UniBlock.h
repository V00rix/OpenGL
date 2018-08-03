//
// Created by vlado on 03-Aug-18.
//

#ifndef OPENGL_UNIFORMBLOCK_H
#define OPENGL_UNIFORMBLOCK_H


#include <GL/glew.h>
#include "../Program/Program.h"

struct UniArg {
    const GLvoid *pointer;
    unsigned size;
};

class UniBlock {

    unsigned ubo{0};
public:
    UniBlock(const char *, unsigned, int, ...);

    void set(int count...) const;
};


#endif //OPENGL_UNIFORMBLOCK_H
