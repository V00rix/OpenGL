//
// Created by vlado on 19-Jul-18.
//

#ifndef OPENGL_MAT4_H
#define OPENGL_MAT4_H

#include <iostream>
#include "vec4.h"

template<typename T>
struct mat4 {
    vec4<T> val[4];

    friend std::ostream &operator<<(std::ostream &output, const mat4 &mat) {
        for (char i = 0; i < 4; i++) {
            output << mat.val[i];
        }
        output << std::endl;
        return output;
    }

    inline vec4<T> operator*(const vec4<T> &vec) {
        return vec4<T> {
            get_part(vec, val[0]),
            get_part(vec, val[1]),
            get_part(vec, val[2]),
            get_part(vec, val[3])
        };
    }

private:
    inline T get_part(const vec4<T> &v1, const vec4<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }
};

#endif //OPENGL_MAT4_H
