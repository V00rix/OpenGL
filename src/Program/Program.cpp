//
// Created by vlado on 24-Jul-18.
//

#include <sstream>
#include <fstream>
#include <vector>
#include "Program.h"

static int infoLogLength;

const Program *Program::active = nullptr;

unsigned Program::createShader(Program::Shaders type, const char *filePath) {
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
        crashGL("Couldn't compile shader");
    }

    printf("Shader '%d' successfully compiled.\n", shaderID);
    return shaderID;
}

Program::Program(const char *vertexFilePath, const char *fragmentFilePath, const char *geometryFilePath) {
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
        if (shader != 0)
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
    for (GLuint shader: shaders) {
        if (shader != 0) {
            glDetachShader(programID, shader);
            glDeleteShader(shader);
        }
    }

    id = programID;
    printf("created program: %d\n", id);
}

void Program::  use() const {
    glUseProgram(id);
    active = this;
}

void Program::cleanGL() const {
    glDeleteProgram(id);
}

//region Uniforms

void Program::setBool(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name), (int) value);
}

void Program::setInt(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Program::setFloat(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Program::setVec2(const char *name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Program::setVec2(const char *name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, name), x, y);
}

void Program::setVec3(const char *name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Program::setVec3(const char *name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void Program::setVec4(const char *name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Program::setVec4(const char *name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

void Program::setMat2(const char *name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

void Program::setMat3(const char *name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

void Program::setMat4(const char *name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

void Program::printData() const {
    printf("Printing data for program (%x):\n", this);
    printf("\tid = %d\n", id);
}

//endregion
const Program &getProgram() {
    if (Program::active != nullptr) {
        return (*Program::active);
    }

    printf("[Attempted to use a program while none is active]\n");
    exit(GL_CRASH_CODE);
}
