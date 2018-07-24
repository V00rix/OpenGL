//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLRENDERER_H
#define OPENGL_GLRENDERER_H


#include <GL/glew.h>
#include <vector>
#include "../GLContext/GLContext.h"
#include "../Shader/Shader.h"


class GLWindow {
    GLContext context;

    std::vector<GLuint> shaderPrograms;
    std::vector<GLuint> textures;

    enum Shaders {
        vertex = GL_VERTEX_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        fragment = GL_FRAGMENT_SHADER
    };

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
     * Loads texture into memory
     * @param filePath Path to the texture file
     * @return Texture index
     */
    unsigned loadTexture(const char *filePath);

    /**
     * Create new rendering context
     * @param context Valid window context
     */
    explicit GLWindow(const GLContext &context);

    virtual ~GLWindow();

};


#endif //OPENGL_GLRENDERER_H
