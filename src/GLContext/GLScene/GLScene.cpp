//
// Created by vlado on 24-Jul-18.
//

#include <glm/gtc/matrix_transform.hpp>
#include "GLScene.h"

void GLScene::render() const {
    const Element *el = head;

    while (el) {
        el->element.render();
        el = el->next;
    }
}

void GLScene::addElement(const elements::ElementBase &element) {
    if (!head) {
        head = new Element(element);
        tail = head;
    } else {
        tail->next = new Element(element);
        tail = tail->next;
    }
}

GLScene::~GLScene() {
    delete head;
}

void GLScene::afterRender() const {

}

void GLScene::beforeRender() const {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    if (depthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    if (blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if (cullFace) {
        glEnable(GL_CULL_FACE);
    }

    int u_world = glGetUniformLocation(program, uniforms.matrix_world);
    glUniformMatrix4fv(u_world, 1, GL_FALSE, &world.mat[0][0]);

    int u_view = glGetUniformLocation(program, uniforms.matrix_view);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &view.mat[0][0]);

    int u_projection = glGetUniformLocation(program, uniforms.matrix_projection);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection.mat[0][0]);

    int u_directional_count = glGetUniformLocation(program, uniforms.lights.directional_count);
    glUniform1i(u_directional_count, lights.directional.size());

    int u_point_count = glGetUniformLocation(program, uniforms.lights.point_count);
    glUniform1i(u_point_count, lights.point.size());

    // todo: figure out what texture sampler is
    int u_texture_sampler = glGetUniformLocation(program, uniforms.texture_sampler);
    glUniform1i(u_texture_sampler, 0);

    // todo: from material
    int u_specular_intensity = glGetUniformLocation(program, uniforms.specular_intensity);
    glUniform1f(u_specular_intensity, 5.0f);

    // todo: from material
    int u_specular_power = glGetUniformLocation(program, uniforms.specular_power);
    glUniform1f(u_specular_power, 32);

    int u_camera_position = glGetUniformLocation(program, uniforms.camera_position);
    glUniform3f(u_camera_position, viewPosition.x, viewPosition.y, viewPosition.z);

    for (auto i = 0; i != lights.directional.size(); i++) {
        std::string s = uniforms.lights.directional;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setDirectional(light::uni::getDirectional(program, s.c_str()), lights.directional[i]);
    }

    for (auto i = 0; i != lights.point.size(); i++) {
        std::string s = uniforms.lights.point;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setPoint(light::uni::getPoint(program, s.c_str()), lights.point[i]);
    }

}

void GLScene::addLight(const light::Directional &directional) {
    this->lights.directional.push_back(directional);
}

void GLScene::addLight(const light::Point &point) {
    this->lights.point.push_back(point);

}

void GLScene::setView(glm::vec3 position, glm::vec3 lookAt, glm::vec3 head) {
    view.mat = glm::lookAt(position, lookAt, head);
    viewPosition = position;
}
