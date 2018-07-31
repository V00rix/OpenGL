//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLRENDERER_H
#define OPENGL_GLRENDERER_H


#include <GL/glew.h>
#include <vector>
#include "../GLWindow/GLWindow.h"
#include "GLScene/GLScene.h"
#include "GLInputHandler/GLInputHandler.h"


class GLContext {
    const GLWindow *context;
    const GLScene *scene;
    const GLInputHandler *inputHandler;
    std::vector<GLuint> textures;

    bool shouldBreak = false;
public:
    bool printFps = false;

    /**
     * Create new rendering context
     * @param context Valid window context
     */
    explicit GLContext(const GLWindow *context);

    /**
     * Set scene
     * @param scene Scene
     */
    void attachScene(const GLScene *scene);

    void render();

    void breakLoop();

    void setInputHandler(const GLInputHandler * handler);
};


#endif //OPENGL_GLRENDERER_H
