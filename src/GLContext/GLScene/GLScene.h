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

    glm::vec4 clearColor = glm::vec4(0.0f);
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

    //            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
//
//            // Camera matrix
//            glm::mat4 view = glm::lookAt(
//                    cameraPosition,
//                    glm::vec3(0, 0, 0), // and looks at the origin
//                    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
//            );
//
//            // model matrix : an identity matrix (model will be at the origin)
//            glm::mat4 model = glm::mat4(1.0f);
//
//            glm::mat4 translated = glm::translate(projection, glm::vec3(.0f, .0f, 0.f));

//            glUniformMatrix4fv(u_model, 1, GL_FALSE, &rotated[0][0]);
//            glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
//            glUniformMatrix4fv(u_projection, 1, GL_FALSE, &translated[0][0]);

    virtual ~GLScene();

    void render() const;

    void beforeRender() const;

    void afterRender() const;

    mutable unsigned int program = 0;

    void addLight(const light::Directional &);

    void addLight(const light::Point &);

//    void addLight(const light::Spot& spot);
};


#endif //OPENGL_GLSCENE_H
