//
// Created by vlado on 24-Jul-18.
//

#include <glm/gtc/matrix_transform.hpp>
#include "GLScene.h"
#include <glfw3.h>

static void printVertex(const glm::vec3 &vertex) {
    std::cout << "\tPosition at: {" << vertex.x << ", " << vertex.y << ", " << vertex.z << "}\n";
}

void GLScene::render() const {

    renderFunc();

    (*activeProgram).setMat4(uniforms.matrix_world, world.mat);
    (*activeProgram).setMat4(uniforms.matrix_view, view.mat);
    (*activeProgram).setMat4(uniforms.matrix_projection, projection.mat);

    if (renderGrid) {
        (*activeProgram).setInt(uniforms.grid_enabled, true);
        grid.render();
        (*activeProgram).setInt(uniforms.grid_enabled, false);
    }

    const Element *el = head;

    while (el) {
        el->element.render();
        el = el->next;
    }

    // render light meshes
    (*activeProgram).setBool(uniforms.light_mesh, true);
    for (int i = 0; i < lights.point.size(); i++) {
        (*activeProgram).setBool(uniforms.lights.point_index, i);
        lights.meshes[i].render();
    }
    (*activeProgram).setBool(uniforms.light_mesh, false);
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
    if (stencilTest) {
//        glEnable(GL_STENCIL_TEST);
//        glStencilMask(0xFF); // each bit is written to the stencil buffer as is
//        glStencilMask(0x00);
//        glStencilFunc(GL_EQUAL, 1, 0xFF);
    }

    if (cullFace) {
        glEnable(GL_CULL_FACE);
    }

    if (renderGrid) {
        grid.init();
    }

    (*activeProgram).use();

    (*activeProgram).setMat4(uniforms.matrix_world, world.mat);
    (*activeProgram).setMat4(uniforms.matrix_view, view.mat);
    (*activeProgram).setMat4(uniforms.matrix_projection, projection.mat);

    (*activeProgram).setInt(uniforms.lights.directional_count, lights.directional.size());
    (*activeProgram).setInt(uniforms.lights.point_count, lights.point.size());
    (*activeProgram).setInt(uniforms.lights.spot_count, lights.spot.size());

    (*activeProgram).setInt(uniforms.texture_sampler, currentTextureIndex);

    // todo: from material
    (*activeProgram).setFloat(uniforms.specular_intensity, 5.0f);
    // todo: from material
    (*activeProgram).setFloat(uniforms.specular_power, 32);

    (*activeProgram).setVec3(uniforms.camera_position, viewPosition);

    // set directional lights
    for (auto i = 0; i != lights.directional.size(); i++) {
        std::string s = uniforms.lights.directional;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setDirectional(light::uni::getDirectional((GLuint) (*activeProgram), s.c_str()), lights.directional[i]);
    }

    // set point lights
    lights.meshes = new elements::Mesh[lights.point.size()];
    for (auto i = 0; i != lights.point.size(); i++) {
        std::string s = uniforms.lights.point;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setPoint(light::uni::getPoint((GLuint) (*activeProgram), s.c_str()), lights.point[i]);

        lights.meshes[i].set(lights.mesh);
        lights.meshes[i].setPosition(lights.point[i].position);
    }

    // set spot lights
    for (auto i = 0; i != lights.spot.size(); i++) {
        std::string s = uniforms.lights.spot;
        s += "[";
        s += std::to_string(i);
        s += "]";
        light::uni::setSpot(light::uni::getSpot((GLuint) (*activeProgram), s.c_str()), lights.spot[i]);
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
    (*activeProgram).setInt(uniforms.texture_sampler, currentTextureIndex = index);
}

void GLScene::addSpotlight(light::Spot spot) {
    this->lights.spot.push_back(spot);
}

void GLScene::useProgram(Program* program) {
    this->activeProgram = program;
    (*program).use();
}

