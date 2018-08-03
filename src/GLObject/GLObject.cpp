//
// Created by vlado on 01-Aug-18.
//

#include "GLObject.h"

void GLObject::crashGL(const char *message) const {
    printf("!-- GLObject error has occurred. this -> (%x)\n", this);
    printf("%s\n", message);
    printData();
    exit(GL_CRASH_CODE);
}
