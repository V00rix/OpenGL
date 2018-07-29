//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_ELEMENTBASE_H
#define OPENGL_ELEMENTBASE_H
#define TRANSL(x) glm::vec3 translation = this->translation;\
                  translate(-translation);\
                  x\
                  translate(translation);

#include <GL/glew.h>
#include "../../util/util.h"

namespace elements {
    class ElementBase {
        glm::vec3 translation;

        void applyVertexTransform() const;

    protected:
        util::Vertex *vertices;
        const unsigned *indices;

        unsigned vertexSize;
        unsigned vertexCount;
        unsigned indexSize;
        unsigned indexCount;

        mutable unsigned VAO;
        mutable unsigned VBO;
        mutable unsigned IBO;

        static const glm::vec2 *uv;

        virtual void initVertices() = 0;

        virtual void initBuffers();

    public:
        ElementBase();

        ElementBase(const ElementBase&);

        virtual ~ElementBase();

        virtual void render() const;

        void rotate(float, const glm::vec3 &);

        void translate(const glm::vec3 &);

        void scale(const glm::vec3 &);

        void setPosition(const glm::vec3&);
    };
}

#endif //OPENGL_ELEMENTBASE_H
