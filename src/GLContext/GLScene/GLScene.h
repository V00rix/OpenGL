//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLSCENE_H
#define OPENGL_GLSCENE_H


#include <glm/mat4x4.hpp>
#include "../../elements/ElementBase/ElementBase.h"

class GLScene {
public:
    struct Element {
        const elements::ElementBase &element;
        Element *next;

        explicit Element(const elements::ElementBase &element) : element(element), next(nullptr) {
            printf("constructing\n");
        }

        virtual ~Element() {
            printf("destructing\n");
            delete next;
        }
    };

private:
    struct Uniforms {
        const char *matrix_world;
        const char *matrix_view;
        const char *matrix_projection;
        const char *camera_position;
        const char *texture_sampler;
        const char *grid_enabled;
        const char *specular_intensity;
        const char *specular_power;
        struct {
            const char *directional_count;
            const char *directional;
            const char *point_count;
            const char *point;
        } lights;
    };

    /**
     * Scene elements to render
     */

    Element *head = nullptr;
    Element *tail = head;
public:
    struct Lights {
        std::vector<light::Directional> directional;
        std::vector<light::Point> point;
//        std::vector<light::Spot> spot;
    } lights;

    class Grid {
        mutable unsigned VAO;
        mutable unsigned VBO;

        mutable unsigned indexCount;
    public:

        /**
         * Highest distance between two grid lines
         */
        float size;

        /**
         * Lines count
         */
        unsigned precision;

        void render() const;

        void init() const;

        Grid(float size, unsigned precision) : size(size), precision(precision), VAO(0), VBO(0), indexCount(0) {}

        Grid() : size(0), precision(0), VAO(0), VBO(0), indexCount(0) {}
    } grid;

    glm::vec4 clearColor = glm::vec4(0.0f);

    bool renderGrid = false;
    bool depthTest = true;
    bool blend = true;
    bool cullFace = true;

    Uniforms uniforms;

    /**
     * Scene matrices class
     */
    class Matrix {
    public:
        glm::mat4x4 mat = glm::mat4(1.0f);

        void translate(glm::vec3);

        void scale();

        void rotate();
    } world, view, projection;

    inline Element *getElements() const { return head; }

    void addElement(const elements::ElementBase &);

    virtual ~GLScene();

    void render() const;

    void beforeRender() const;

    void afterRender() const;

    mutable unsigned int program = 0;

    void addLight(const light::Directional &);

    void addLight(const light::Point &);

//    void addLight(const light::Spot& spot);

    void setView(glm::vec3 position, glm::vec3 lookAt, glm::vec3 head);

    glm::vec3 viewPosition;

};


#endif //OPENGL_GLSCENE_H
