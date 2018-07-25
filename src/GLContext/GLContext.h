//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLRENDERER_H
#define OPENGL_GLRENDERER_H


#include <GL/glew.h>
#include <vector>
#include "../GLWindow/GLWindow.h"
#include "../Shader/Shader.h"
#include "GLScene/GLScene.h"
#include "GLInputHandler/GLInputHandler.h"


class GLContext {
    const GLWindow *context;
    const GLScene *scene;
    const GLInputHandler *inputHandler;
public:
    const GLInputHandler *getInputHandler() const;

private:

    std::vector<GLuint> shaderPrograms;
    std::vector<GLuint> textures;

    enum Shaders {
        vertex = GL_VERTEX_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        fragment = GL_FRAGMENT_SHADER
    };

    bool shouldBreak = false;

    /**
     * Load shader code from file and compile it
     * @param type Shader type @see GLWindow::Shaders
     * @param filePath Path to shader file
     * @return Shader index
     */
    GLuint createShader(Shaders type, const char *filePath);

public:

    /**
     * Create program from shaders
     * @param vertexFilePath Path to vertex shader file
     * @param geometryFilePath Path to geometry shader file
     * @param fragmentFilePath Path to fragment shader file
     * @return Program index
     */
    unsigned createProgram(const char *vertexFilePath,
                           const char *geometryFilePath,
                           const char *fragmentFilePath);

    /**
     * Use shader program
     * @param program Program index
     */
    void useProgram(unsigned int program);

    /**
     * Loads texture into memory
     * @param filePath Path to the texture file
     * @return Texture index
     */
    unsigned loadTexture(const char *filePath);

    /**
     * Create new rendering context
     * @param context Valid window context
     */
    explicit GLContext(const GLWindow *context);

    virtual ~GLContext();

    /**
     * Set scene
     * @param scene Scene
     */
    void attachScene(const GLScene *scene);

    /**
     * Get scene
     * @return
     */
    const GLScene *getScene() const;

    void render();

    void breakLoop();

    void setInputHandler(const GLInputHandler * handler);

    unsigned int program;
};


#endif //OPENGL_GLRENDERER_H
