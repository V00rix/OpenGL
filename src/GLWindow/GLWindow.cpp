//
// Created by vlado on 24-Jul-18.
//

#include "GLWindow.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "../util/util.h"

static int infoLogLength;

GLWindow::GLWindow(const GLContext &context) : context(context) {
}

GLWindow::~GLWindow() {
}

unsigned GLWindow::createProgram(const char *vertexFilePath,
                                 const char *geometryFilePath,
                                 const char *fragmentFilePath) {
    infoLogLength = 0;

    const GLuint shaders[3] = {
            createShader(Shaders::vertex, vertexFilePath),
            createShader(Shaders::geometry, geometryFilePath),
            createShader(Shaders::fragment, fragmentFilePath)
    };

    // Link program
    printf("Linking program\n");
    GLuint programID = glCreateProgram();

    for (GLuint shader: shaders) {
        glAttachShader(programID, shader);
    }
    glLinkProgram(programID);

    // Check program
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> ProgramErrorMessage((unsigned) infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    // Cleanup
    for (GLuint i: shaders) {
        glDetachShader(programID, shaders[i]);
        glDeleteShader(shaders[i]);
    }

    shaderPrograms.push_back(programID);

    return programID;
}

GLuint GLWindow::createShader(GLWindow::Shaders type, const char *filePath) {
    if (filePath == nullptr) return (GLuint) nullptr;

    // Create shader
    GLuint shaderID = glCreateShader(type);

    // Read shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(filePath, std::ios::in);
    if (shaderStream.is_open()) {
        std::stringstream stringStream;
        stringStream << shaderStream.rdbuf();
        shaderCode = stringStream.str();
        shaderStream.close();
    } else {
        printf("Couldn't open shader file: %s.\n", filePath);
        getchar();
        return 0;
    }

    //  Compile shader
    printf("Compiling shader : %s\n", filePath);
    char const *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, nullptr);
    glCompileShader(shaderID);

    // Check for errors
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> shaderErrorMessage((unsigned) infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &shaderErrorMessage[0]);
        printf("%s\n", &shaderErrorMessage[0]);
    }

    printf("Shader '%d' successfully compiled.\n", shaderID);
    return shaderID;
}

unsigned GLWindow::loadTexture(const char *filePath) {

    util::FExtension ext = util::getFileExtension(filePath);

    GLuint texture;
    switch (ext) {
        case util::bmp:
            texture = util::loadBMP(filePath);
            textures.push_back(texture);
            return texture;
        case util::dds:
            texture = util::loadDDS(filePath);
            textures.push_back(texture);
            return texture;
        default:
            return 0;
    }
}
