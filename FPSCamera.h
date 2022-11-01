#ifndef _FPS_CAMERA_H_
#define _FPS_CAMERA_H_

#include <iostream>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


enum Direction {
    NONE, FORWARD, BACKWARD, LEFT, RIGHT
};

class FPSCamera {
public:
    
    ~FPSCamera();

    float sensitivity = 0.1f;
    float speed = 0.5f;
    float pitch = 0;
    float yaw = -90.0f;
    glm::vec3 pos = glm::vec3(1.0f);
    glm::vec3 front = glm::vec3(1.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::mat4(1.0f);

    
    float fov = 45.0f;
    float fovRatio = 800.0f / 400;
    glm::mat4 projection = glm::mat4(1.0f);

    void updateDirection(float pitchOffset, float yawOffset);

    void updatePos(Direction direction, float durationS);

    void updateFov(float fovOffset);

    void update();

    class Builder {
    public:
        Builder& setPitch(float pitch);
        Builder& setYaw(float yaw);
        Builder& setSensitivity(float sensitivity);
        Builder& setPos(glm::vec3 pos);
        Builder& setWorldUp(glm::vec3 up);
        Builder& setSpeed(float speed);
        Builder& setFov(float fov);
        Builder& setFovRation(float ratio);

        FPSCamera build();
    private:
        float sensitivity = 0.1f;
        float pitch = 0;
        float yaw = -90.0f;
        glm::vec3 pos = glm::vec3(1.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float speed = 0.5f;
        float fov = 45.0f;
        float fovRatio = 800.0f / 400;
    };

private:
    FPSCamera();
    void computeViewMat();
    void computeProjectionMat();
};

#endif