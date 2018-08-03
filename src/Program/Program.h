//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H


#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../GLObject/GLObject.h"

class Program : GLObject {
    enum Shaders {
        vertex = GL_VERTEX_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        fragment = GL_FRAGMENT_SHADER
    };

    unsigned id{0};

    unsigned createShader(Shaders, const char *);

public:
    Program() = default;

    Program(const char *vertexFilePath,
            const char *fragmentFilePath,
            const char *geometryFilePath = nullptr);

    void use() const;

    void cleanGL() const override;

    void printData() const override;

    explicit operator unsigned() const { return id; }

    explicit operator int() const { return id; }

    // getProgram program reference
    static const Program *active;

    //region Uniforms

    void setBool(const char *name, bool value) const;

    void setInt(const char *name, int value) const;

    void setFloat(const char *name, float value) const;

    void setVec2(const char *name, const glm::vec2 &value) const;

    void setVec2(const char *name, float x, float y) const;

    void setVec3(const char *name, const glm::vec3 &value) const;

    void setVec3(const char *name, float x, float y, float z) const;

    void setVec4(const char *name, const glm::vec4 &value) const;

    void setVec4(const char *name, float x, float y, float z, float w);

    void setMat2(const char *name, const glm::mat2 &mat) const;

    void setMat3(const char *name, const glm::mat3 &mat) const;

    void setMat4(const char *name, const glm::mat4 &mat) const;
    //endregion
};

const Program &getProgram();

#endif //OPENGL_SHADER_H
