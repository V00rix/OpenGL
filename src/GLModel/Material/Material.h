//
// Created by vlado on 01-Aug-18.
//

#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H


#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include "../../GLObject/GLObject.h"

class Material : GLObject {
    glm::vec3 ambient{0.f}, diffuse{0.f}, specular{0.f};
    float shininess{0.f};

    void printData() const override;
public:
    void cleanGL() const override;
    Material() = default;

    void use() const;
};


#endif //OPENGL_MATERIAL_H
