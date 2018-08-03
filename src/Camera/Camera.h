//
// Created by vlado on 02-Aug-18.
//

#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>
#include "../UniBlock/UniBlock.h"

// Camera movement possibilities
enum CameraMovement {
    FORWARD, BACKWARD, LEFT, RIGHT
};

// Defaults camera values
const float YAW = -90.f, PITCH = 0.f, SPEED = 2.f, SENSITIVITY = 0.1f, ZOOM = 45.f;
const glm::vec3 POSITION{2.f, 2.f, -2.f}, FRONT{0.f, 0.f, 0.f};

// camera head
const glm::vec3 HEAD{0.f, 1.f, 0.f};

class Camera {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::mat4 world{1.f};

    glm::vec3 front{FRONT};
    glm::vec3 position{POSITION};

    float yaw{YAW}, pitch{PITCH}, speed{SPEED}, sensitivity{SENSITIVITY}, _zoom{ZOOM};

    glm::mat4 calculatePerspective(float zoom) const;

public:
    explicit Camera(const glm::vec3 &position, const glm::vec3 &front, float _zoom = ZOOM);

    explicit Camera(const glm::vec3 &position = POSITION, float _zoom = ZOOM) : Camera(position, FRONT, _zoom) {}

    explicit Camera(float zoom, const glm::vec3 &front = FRONT) : Camera(POSITION, front, zoom) {}

    //region Repositioning
    void zoom(float zoom);

    void positionAt(const glm::vec3 &position);

    void lookAt(const glm::vec3 &lookAt);

    void move(CameraMovement direction, float deltaTime) const;

    void rotate(float x, float y) const;
    //endregion

    /**
     * Set camera in space for current frame
      * @param matrices Matrices uniform block
      */
    void set(const UniBlock &matrices) const;
};


#endif //OPENGL_CAMERA_H
