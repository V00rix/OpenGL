//
// Created by vlado on 19-Jul-18.
//

#ifndef OPENGL_VECTOR_H
#define OPENGL_VECTOR_H

template<typename T>
class Vector {
public:
    const T *values;
    const int count;
    const int size;
    Vector(const T *values, const int count) : values(values), count(count), size(count * sizeof(T)) {}

    Vector(T arr[], const int count): values(arr), count(count), size(count * sizeof(T)) {}

    virtual ~Vector() {}
};


#endif //OPENGL_VECTOR_H
