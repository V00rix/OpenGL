//
// Created by vlado on 24-Jul-18.
//

#include <glm/gtc/matrix_transform.hpp>
#include "GLScene.h"
#include <glfw3.h>

static int u_light_mesh;
static int u_grid;
static int u_world;
static int u_view;
static int u_projection;
static int u_point_index;
static int u_directional_index;
static int u_texture_sampler;

static void printVertex(const glm::vec3 &vertex) {
    std::cout << "\tPosition at: {" << vertex.x << ", " << vertex.y << ", " << vertex.z << "}\n";
}

void GLScene::render() const {
    renderFunc();
    glUniformMatrix4fv(u_world, 1, GL_FALSE, &world.mat[0][0]);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &view.mat[0][0]);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection.mat[0][0]);

    if (renderGrid) {
        glUniform1i(u_grid, true);
        grid.render();
        glUniform1i(u_grid, false);
    }

    const Element *el = head;

    while (el) {
        el->element.render();
        el = el->next;
    }

    // render light meshes
    glUniform1i(u_light_mesh, true);
    for (int i = 0; i < lights.point.size(); i++) {
        glUniform1i(u_point_index, i);
        lights.meshes[i].render();
    }
    glUniform1i(u_light_mesh, false);

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
    delete[] lights.meshes;
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

    if (renderGrid) {
        grid.init();
    }


    u_grid = glGetUniformLocation(program, uniforms.grid_enabled);

    u_world = glGetUniformLocation(program, uniforms.matrix_world);
    glUniformMatrix4fv(u_world, 1, GL_FALSE, &world.mat[0][0]);

    u_view = glGetUniformLocation(program, uniforms.matrix_view);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &view.mat[0][0]);

    u_projection = glGetUniformLocation(program, uniforms.matrix_projection);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection.mat[0][0]);

    int u_directional_count = glGetUniformLocation(program, uniforms.lights.directional_count);
    glUniform1i(u_directional_count, lights.directional.size());

    int u_point_count = glGetUniformLocation(program, uniforms.lights.point_count);
    glUniform1i(u_point_count, lights.point.size());

    int u_spot_count = glGetUniformLocation(program, uniforms.lights.spot_count);
    glUniform1i(u_spot_count, lights.spot.size());

    u_texture_sampler = glGetUniformLocation(program, uniforms.texture_sampler);
    glUniform1i(u_texture_sampler, currentTextureIndex);

    // todo: from material
    int u_specular_intensity = glGetUniformLocation(program, uniforms.specular_intensity);
    glUniform1f(u_specular_intensity, 5.0f);

    // todo: from material
    int u_specular_power = glGetUniformLocation(program, uniforms.specular_power);
    glUniform1f(u_specular_power, 32);

    int u_camera_position = glGetUniformLocation(program, uniforms.camera_position);
    glUniform3f(u_camera_position, viewPosition.x, viewPosition.y, viewPosition.z);

    u_light_mesh = glGetUniformLocation(program, uniforms.light_mesh);
    u_point_index = glGetUniformLocation(program, uniforms.lights.point_index);
    u_directional_index = glGetUniformLocation(program, uniforms.lights.directional_index);

    // set directional lights
    for (auto i = 0; i != lights.directional.size(); i++) {
        std::string s = uniforms.lights.directional;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setDirectional(light::uni::getDirectional(program, s.c_str()), lights.directional[i]);
    }

    // set point lights
    lights.meshes = new elements::Mesh[lights.point.size()];
    for (auto i = 0; i != lights.point.size(); i++) {
        std::string s = uniforms.lights.point;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setPoint(light::uni::getPoint(program, s.c_str()), lights.point[i]);

        lights.meshes[i].set(lights.mesh);
        lights.meshes[i].setPosition(lights.point[i].position);
    }

    // set spot lights
    for (auto i = 0; i != lights.spot.size(); i++) {
        std::string s = uniforms.lights.spot;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setSpot(light::uni::getSpot(program, s.c_str()), lights.spot[i]);
        // todo spotlight meshes
//        lights.meshes[i].set(lights.mesh);
//        lights.meshes[i].setPosition(lights.point[i].position);
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

void GLScene::setLightMesh(const elements::Mesh &mesh) {
    lights.mesh = mesh;
}

void GLScene::useTexture(unsigned index) {
    glUniform1i(u_texture_sampler, currentTextureIndex = index);
}

void GLScene::addSpotlight(light::Spot spot) {
    this->lights.spot.push_back(spot);
}

