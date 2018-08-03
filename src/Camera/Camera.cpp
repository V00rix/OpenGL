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

void Camera::move(CameraMovement direction, float deltaTime) {
    glm::vec3 right{glm::normalize(glm::cross(front, up))};

    float velocity = speed;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;

    updateView();
}

void Camera::rotate(float x, float y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x;
    lastY = y;

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = (float) (cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front.y = (float) sin(glm::radians(pitch));
    front.z = (float) (sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    this->front = glm::normalize(front);

    updateView();
}

glm::mat4 Camera::calculatePerspective(float zoom) const {
    return glm::perspective(glm::radians(zoom), 4.0f / 3.0f, 0.1f, 100.0f);
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &lookAt, float _zoom) :
        position{position}, front{glm::normalize(lookAt - position)}, projection{calculatePerspective(_zoom)},
        view{glm::lookAt(position, lookAt, UP)} {
    pitch = (float) asin(front.y);
    yaw = glm::degrees(std::acos(front.x / std::cos(pitch)));
    pitch = glm::degrees(pitch);
}

void Camera::updateView() {
    view = glm::lookAt(position, position + front, up);
}
