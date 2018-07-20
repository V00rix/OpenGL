//
// Created by vlado on 20-Jul-18.
//

#include "Cube.h"
#include <gl/glew.h>
#include <iostream>

elements::Cube::Cube(const elements::Cube::point_f &at, float edge_length) {
    std::cout << "Constructing cube\n";

    positions = new point_f[vertexCount]{
            // lower square
            {at.x,               at.y,               at.z},                 // 0
            {at.x + edge_length, at.y,               at.z},                 // 1
            {at.x + edge_length, at.y,               at.z + edge_length},   // 2
            {at.x,               at.y,               at.z + edge_length},   // 3

            // higher square
            {at.x,               at.y + edge_length, at.z},                 // 4
            {at.x + edge_length, at.y + edge_length, at.z},                 // 5
            {at.x + edge_length, at.y + edge_length, at.z + edge_length},   // 6
            {at.x,               at.y + edge_length, at.z + edge_length},   // 7

    };

    indices = new unsigned[indexCount]{
            // bottom
            0, 1, 2,
            0, 2, 3,

            // top
            4, 5, 6,
            4, 6, 7,

            // right
            1, 2, 6,
            1, 5, 6,

            // left
            0, 3, 4,
            4, 3, 7,

            // front
            0, 1, 5,
            0, 4, 5,

            // back
            2, 3, 7,
            2, 7, 6
    };

    colors = new point_f[8];
    setColors({.5f, .0f, .0f});
}

elements::Cube::~Cube() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &vertexArray);

    delete[] this->positions;
    delete[] this->colors;
    delete[] this->indices;
}

void elements::Cube::setColors(const point_f *colors) {
    for (int i = 0; i < 8; ++i) {
        this->colors[i] = colors[i];
    }
}

void elements::Cube::setColors(const point_f &color) {
    for (int i = 0; i < 8; ++i) {
        this->colors[i] = color;
    }
}

void elements::Cube::init() const {
    // buffers
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, positions, GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, colors, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

}

void elements::Cube::draw() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}