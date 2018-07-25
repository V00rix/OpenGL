//
// Created by vlado on 21-Jul-18.
//

#ifndef OPENGL_ELEMENTBASE_H
#define OPENGL_ELEMENTBASE_H

#include <GL/glew.h>
#include "../../util/util.h"

namespace elements {
    class ElementBase {
    protected:
        util::Vertex *vertices;
        const unsigned *indices;

        unsigned vertexSize;
        unsigned indexSize;
        unsigned indexCount;

        mutable unsigned VAO;
        mutable unsigned VBO;
        mutable unsigned IBO;

        static const glm::vec2 *uv;

        virtual void initVertices()=0;

        void initBuffers();

    public:
        virtual ~ElementBase();

        inline virtual void render() const {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        };
    };
}

#endif //OPENGL_ELEMENTBASE_H
