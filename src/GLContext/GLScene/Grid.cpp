//
// Created by vlado on 29-Jul-18.
//


#include "GLScene.h"

void GLScene::Grid::render() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, indexCount);
}

void GLScene::Grid::init() const {
    float delta = size / ((float) precision * 2);

    unsigned linePointsCount = 1 + precision * 2;
    glm::vec3 vertices[linePointsCount * 4];

    struct {
        struct {
            glm::vec3 left, right;
        } bottom;
        struct {
            glm::vec3 left, right;
        } top;

    } start{
            {
                    {-size / 2, 0, -size / 2},
                    {size / 2, 0, -size / 2}
            },
            {
                    {-size / 2, 0, size / 2},
                    {size / 2, 0, size / 2}
            }
    };

    // vertical
    indexCount = 0;
    for (unsigned i = 0; i < linePointsCount; i++) {
        vertices[indexCount++] = {
                start.bottom.left.x + delta * i,
                start.bottom.left.y,
                start.bottom.left.z
        };
        vertices[indexCount++] = {
                start.top.left.x + delta * i,
                start.top.left.y,
                start.top.left.z
        };
    }

    // horizontal
    for (unsigned i = 0; i < linePointsCount; i++) {
        vertices[indexCount++] = {
                start.bottom.left.x,
                start.bottom.left.y,
                start.bottom.left.z + delta * i
        };
        vertices[indexCount++] = {
                start.bottom.right.x,
                start.bottom.right.y,
                start.bottom.right.z + delta * i
        };
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

}
