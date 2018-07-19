//
// Created by vlado on 19-Jul-18.
//

#ifndef OPENGL_VEC4_H
#define OPENGL_VEC4_H

#include <iostream>

template<typename T>
struct vec4 {
    T x, y, z, w;

    friend std::ostream &operator<<(std::ostream &output, const vec4 &vec) {
        for (char i = 0; i < 4; i++) {
            T j = *((T*)(&vec) + i);
            output << j;
            output << " ";
        }
        output << std::endl;
        return output;
    }
};

#endif //OPENGL_VEC4_H
