//
// Created by vlado on 03-Aug-18.
//

#include <cstdarg>
#include "UniBlock.h"

void UniBlock::set(int count, ...) const {
    if (!ubo) {
        printf("[Trying to set uninitialized uniform block]\n");
        exit(GL_CRASH_CODE);
    }

    va_list args;
    va_start(args, count);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    unsigned offset = 0;

    for (int i = 0; i < count; i++) {
        UniArg arg = va_arg(args, UniArg);

        glBufferSubData(GL_UNIFORM_BUFFER, offset, arg.size, arg.pointer);
        offset += arg.size;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniBlock::UniBlock(const Program &program, const char *name, unsigned size) {
    unsigned id = glGetUniformBlockIndex((unsigned) program, name);
    glUniformBlockBinding((unsigned) program, id, 0);

    ubo;
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, size);
}
