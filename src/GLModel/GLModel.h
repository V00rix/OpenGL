//
// Created by vlado on 01-Aug-18.
//

#ifndef OPENGL_GLELEMENT_H
#define OPENGL_GLELEMENT_H


#include "Material/Material.h"
#include "Mesh/Mesh.h"
#include "../Texture/Texture.h"
#include "../GLObject/GLObject.h"
#include <functional>

class GLModel : GLObject {
    Mesh mesh;
    Material material;
    Texture texture; // Textures, light, parallax, normal maps etc.

    void printData() const override;

public:
    explicit GLModel(const Mesh &mesh) { setMesh(mesh); }

    explicit GLModel(const Mesh &mesh, const Texture &texture) {
        setMesh(mesh);
        setTexture(texture);
    }

    virtual void render() const;

    inline void setMaterial(const Material &material) { this->material = material; }

    inline void setTexture(const Texture &texture) { this->texture = texture; }

    inline void setMesh(const Mesh &mesh) { this->mesh = mesh; }

    inline void transform(const glm::mat4 &transformation) { this->mesh.transform(transformation); }

    void cleanGL() const override;
};


#endif //OPENGL_GLELEMENT_H
