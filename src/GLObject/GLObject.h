//
// Created by vlado on 01-Aug-18.
//

#ifndef OPENGL_GLOBJECT_H
#define OPENGL_GLOBJECT_H


#include <cstdio>
#include <cstdlib>

#define GL_CRASH_CODE (-111)

class GLObject {
protected:
    virtual void printData() const = 0;

    void crashGL(const char *message) const;
public:
    virtual void cleanGL() const = 0;
};


#endif //OPENGL_GLOBJECT_H
