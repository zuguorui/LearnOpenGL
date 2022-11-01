#include "FPSCamera.h"

FPSCamera::Builder& FPSCamera::Builder::setPitch(float pitch) {
    this->pitch = pitch;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setYaw(float yaw) {
    this->yaw = yaw;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setPos(glm::vec3 pos) {
    this->pos = pos;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setWorldUp(glm::vec3 worldUp) {
    this->worldUp = worldUp;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setSpeed(float speed) {
    this->speed = speed;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setFov(float fov) {
    this->fov = fov;
    return *this;
}

FPSCamera::Builder& FPSCamera::Builder::setFovRation(float ratio) {
    this->fovRatio = ratio;
    return *this;
}

FPSCamera FPSCamera::Builder::build() {
    FPSCamera camera;
    camera.sensitivity = sensitivity;
    camera.pitch = pitch;
    camera.yaw = yaw;
    camera.pos = pos;
    camera.worldUp = worldUp;
    camera.speed = speed;
    camera.fov = fov;
    camera.fovRatio = fovRatio;
    camera.update();
    return camera;
}

FPSCamera::FPSCamera() {

}

FPSCamera::~FPSCamera() {

}

void FPSCamera::update() {
    updateFov(0);
    updateDirection(0, 0);
    updatePos(Direction::FORWARD, 0);
}

void FPSCamera::updatePos(Direction direction, float durationS) {
    if (direction == Direction::NONE) {
        return;
    }
    float offset = durationS * speed;
    switch (direction)
    {
    case Direction::FORWARD:
        pos += front * offset;
        break;
    case Direction::BACKWARD:
        pos -= front * offset;
        break;
    case Direction::LEFT:
        pos -= glm::normalize(glm::cross(front, worldUp)) * offset;
        break;
    case Direction::RIGHT:
        pos += glm::normalize(glm::cross(front, worldUp)) * offset;
        break;
    default:
        break;
    }

    computeViewMat();
}

void FPSCamera::updateDirection(float pitchOffset, float yawOffset) {
    pitch += pitchOffset * sensitivity;
    yaw += yawOffset * sensitivity;
    if (pitch > 89) {
        pitch = 89.0f;
    }
    if (pitch < -89) {
        pitch = -89.0f;
    }

    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(front);
    computeViewMat();
}

void FPSCamera::updateFov(float fovOffset) {
    fov -= fovOffset;
    if (fov < 1) {
        fov = 1.0f;
    }
    if (fov > 45) {
        fov = 45.0f;
    }
    computeProjectionMat();
}

void FPSCamera::computeViewMat() {
    view = glm::lookAt(pos, pos + front, worldUp);
}

void FPSCamera::computeProjectionMat() {
    projection = glm::perspective(fov, fovRatio, 0.01f, 100.0f);
}



