#include "Camera.h"
#include <algorithm>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera()
    : target(0), distance(80), targetDistance(80), yaw(45), pitch(25),
      targetYaw(45), targetPitch(25), fov(45), aspectRatio(16.0f/9.0f),
      minDistance(5.0f), maxDistance(8000.0f), leftDrag(false), lastX(0), lastY(0) {}

void Camera::update(float dt) {
    distance += (targetDistance - distance) * 10.0f * dt;
    yaw += (targetYaw - yaw) * 10.0f * dt;
    pitch += (targetPitch - pitch) * 10.0f * dt;
}

glm::mat4 Camera::getView() {
    float rYaw = glm::radians(yaw);
    float rPitch = glm::radians(pitch);
    glm::vec3 pos = target + glm::vec3(
        distance * cos(rPitch) * cos(rYaw),
        distance * sin(rPitch),
        distance * cos(rPitch) * sin(rYaw)
    );
    return glm::lookAt(pos, target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 20000.0f);
}