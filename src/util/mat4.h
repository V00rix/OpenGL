//
// Created by vlado on 19-Jul-18.
//

#ifndef OPENGL_MAT4_H
#define OPENGL_MAT4_H

#include <iostream>
#include <cmath>
#include "vec4.h"

namespace mat {
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
            return vec4<T>{
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

    struct scale : mat4<float> {
        scale(float x, float y, float z) : mat4({x, 0, 0, 0,
                                                 0, y, 0, 0,
                                                 0, 0, z, 0,
                                                 0, 0, 0, 1}) {}
    };

    struct translate : mat4<float> {
        translate(float x, float y, float z) : mat4({1, 0, 0, x,
                                                     0, 1, 0, y,
                                                     0, 0, 1, z,
                                                     0, 0, 0, 1}) {}
    };

    struct rotateX : mat4<float> {
        explicit rotateX(float a) : mat4({1, 0, 0, 0,
                                          0, std::cos(a), -std::sin(a), 0,
                                          0, std::sin(a), std::cos(a), 0,
                                          0, 0, 0, 1}) {}
    };

    struct rotateY : mat4<float> {
        explicit rotateY(float a) : mat4({std::cos(a), 0, std::sin(a), 0,
                                          0, 1, 0, 0,
                                          -std::sin(a), 0, std::cos(a), 0,
                                          0, 0, 0, 1}) {}
    };

    struct rotateZ : mat4<float> {
        explicit rotateZ(float a) : mat4({std::cos(a), -std::sin(a), 0, 0,
                                          std::sin(a), std::cos(a), 0, 0,
                                          0, 0, 0, 0,
                                          0, 0, 0, 1}) {}
    };

    struct rotate {
    private:
        rotateX rx;
        rotateY ry;
        rotateZ rz;
        float a, b, c;
    public:

        friend std::ostream &operator<<(std::ostream &output, const rotate &rot) {
            output << "Rotation matrix: (" << rot.a << "deg, " << rot.b << "deg, " << rot.c << "deg)\n";
            return output;
        }

        rotate(float a, float b, float c) : a(a), b(b), c(c), rx(rotateX(a)), ry(rotateY(b)), rz(rotateZ(c)) {}

        inline vec4<float> operator*(const vec4<float> &vec) {
            return rz * (ry * (rx * vec));
        }
    };


}

#endif //OPENGL_MAT4_H
