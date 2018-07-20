//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

namespace elements {

    class Cube {
    private:
        struct point_f {
            float x, y, z;
        };

        point_f *positions;
        point_f *colors;
        const unsigned *indices;

        mutable unsigned vertexArray;
        mutable unsigned vertexBuffer;
        mutable unsigned colorBuffer;
        mutable unsigned indexBuffer;

        const unsigned char vertexCount = 8;
        const unsigned char vertexSize = vertexCount * 3 * sizeof(float);
        const unsigned char indexCount = 36;
        const unsigned char indexSize = indexCount * 3 * sizeof(unsigned);

    public:
        Cube(const point_f &at, float edge_length);

        virtual ~Cube();

        void setColors(const point_f *colors);

        inline void setColors(const float *colors) {
            setColors((point_f *) colors);
        }

        void setColors(const point_f &color);

        void init() const;

        void draw();
    };
}


#endif //OPENGL_CUBE_H
