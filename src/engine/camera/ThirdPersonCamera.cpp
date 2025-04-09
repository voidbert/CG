#include "engine/camera/ThirdPersonCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace engine::camera {
ThirdPersonCamera::ThirdPersonCamera(std::shared_ptr<scene::Entity> _playerEntity,
                                     const glm::vec3 &_position,
                                     const glm::vec3 &_lookAt,
                                     const glm::vec3 &_up,
                                     float _fov,
                                     float _near,
                                     float _far,
                                     float _speed,
                                     float _yaw,
                                     float _pitch,
                                     float _distanceFromPlayer) :
    Camera(_position, _lookAt, _up, _fov, _near, _far),
    yaw(_yaw),
    pitch(_pitch),
    speed(_speed),
    distanceFromPlayer(_distanceFromPlayer),
    playerEntity(std::move(_playerEntity)),
    playerPosition(_lookAt) {
    calculatePosition();
}

void ThirdPersonCamera::update() {
    calculatePosition();

    glm::vec3 forward = glm::normalize(playerPosition - position);
    lookAt = position + forward;

    updateCameraMatrix();
    updateViewFrustum();
}

void ThirdPersonCamera::calculatePosition() {
    float yawRad = glm::radians(yaw);
    float pitchRad = glm::radians(pitch);

    float x = distanceFromPlayer * cos(pitchRad) * sin(yawRad);
    float y = distanceFromPlayer * sin(pitchRad);
    float z = distanceFromPlayer * cos(pitchRad) * cos(yawRad);

    position = playerPosition + glm::vec3(-x, y, -z);
}

glm::mat4 ThirdPersonCamera::getViewMatrix() const {
    return glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 ThirdPersonCamera::getPosition() const {
    return position;
}

void ThirdPersonCamera::renderPlayer(const render::RenderPipeline &pipeline,
                                     const glm::mat4 &initialTransform) const {
    if (playerEntity) {
        glm::mat4 playerTransform = glm::scale(initialTransform, glm::vec3(0.3f, 0.5f, 0.3f));
        playerEntity->draw(pipeline, playerTransform);
    }
}

void ThirdPersonCamera::move(MovementDirection dir, float deltaTime) {
    const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    glm::vec3 forward = glm::normalize(playerPosition - position);
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    glm::vec3 _up = glm::normalize(glm::cross(right, forward));

    if (dir == MovementDirection::Forward) {
        position += forward * speed * deltaTime;
        playerPosition += forward * speed * deltaTime;
    } else if (dir == MovementDirection::Backward) {
        position -= forward * speed * deltaTime;
        playerPosition -= forward * speed * deltaTime;
    } else if (dir == MovementDirection::Left) {
        position += right * speed * deltaTime;
        playerPosition += right * speed * deltaTime;
    } else if (dir == MovementDirection::Right) {
        position -= right * speed * deltaTime;
        playerPosition -= right * speed * deltaTime;
    } else if (dir == MovementDirection::Up) {
        position += _up * speed * deltaTime;
        playerPosition += _up * speed * deltaTime;
    } else if (dir == MovementDirection::Down) {
        position -= _up * speed * deltaTime;
        playerPosition -= _up * speed * deltaTime;
    } else if (dir == MovementDirection::LookLeft) {
        yaw -= 90.0f * deltaTime;
    } else if (dir == MovementDirection::LookRight) {
        yaw += 90.0f * deltaTime;
    } else if (dir == MovementDirection::LookUp) {
        pitch += 45.0f * deltaTime;
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
    } else if (dir == MovementDirection::LookDown) {
        pitch -= 45.0f * deltaTime;
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
    }

    update();
}
}
