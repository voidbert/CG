#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera/Camera.hpp"
#include "engine/scene/Entity.hpp"

namespace engine::camera {

class ThirdPersonCamera : public Camera {

private:
    float yaw;
    float pitch;
    float speed;
    float distanceFromPlayer;

    std::shared_ptr<scene::Entity> playerEntity;
    glm::vec3 playerPosition;

public:
    ThirdPersonCamera(std::shared_ptr<scene::Entity> _playerEntity,
                      const glm::vec3 &_position,
                      const glm::vec3 &_lookAt,
                      const glm::vec3 &_up,
                      float _fov,
                      float _near,
                      float _far,
                      float _speed = 1.0f,
                      float _yaw = 0.0f,
                      float _pitch = 20.0f,
                      float _distanceFromPlayer = 5.0f);

    void update();
    void move(MovementDirection dir, float deltaTime) override;
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    void renderPlayer(const render::RenderPipeline &pipeline,
                      const glm::mat4 &initialTransform) const override;
    void calculatePosition();
};

}
