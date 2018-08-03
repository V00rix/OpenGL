//
// Created by vlado on 02-Aug-18.
//

#include "Camera.h"
#include "../Program/Program.h"
#include "../util/uniforms.h"
#include "../util/util.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::set(const UniBlock &matrices) const {
    matrices.set(3,
                 UniArg{&projection[0][0], sizeof(projection)},
                 UniArg{&view[0][0], sizeof(view)},
                 UniArg{&world[0][0], sizeof(world)});
}

void Camera::positionAt(const glm::vec3 &position) {

}

void Camera::zoom(float zoom) {
    if (this->_zoom >= 1.0f && this->_zoom <= 45.0f)
        this->_zoom -= zoom;
    if (this->_zoom <= 1.0f)
        this->_zoom = 1.0f;
    if (this->_zoom >= 45.0f)
        this->_zoom = 45.0f;

    this->projection = calculatePerspective(zoom);
}

void Camera::lookAt(const glm::vec3 &lookAt) {

}

void Camera::move(CameraMovement direction, float deltaTime) const {

}

void Camera::rotate(float x, float y) const {

}

glm::mat4 Camera::calculatePerspective(float zoom) const {
    return glm::perspective(glm::radians(zoom), 4.0f / 3.0f, 0.1f, 100.0f);
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &front, float _zoom) :
        position{position}, front{front}, projection{calculatePerspective(_zoom)},
        view{glm::lookAt(position, front, HEAD)} {

}
