//
// Created by ayub on 10/18/26.
//

#ifndef OPENGL_SPACETIME_GALAXY_CAMERA_H
#define OPENGL_SPACETIME_GALAXY_CAMERA_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>


class Camera {
    public:
    glm::vec3 target;
    float distance, targetDistance;
    float yaw, pitch, targetYaw, targetPitch;
    float fov, aspectRatio;
    float minDistance, maxDistance;
    bool leftDrag, rightDrag;
    double lastX, lastY;

    Camera();

    void update(float dt);
    glm::mat4 getView();
    glm::mat4 getProjection();

};


#endif //OPENGL_SPACETIME_GALAXY_CAMERA_H
