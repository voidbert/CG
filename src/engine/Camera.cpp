/// Copyright 2025 Ana Oliveira, Humberto Gomes, Mariana Rocha, Sara Lopes
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.

#include "engine/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm> 

namespace engine {

Camera::Camera() : position(0.0f, 0.0f, 3.0f), pitch(0.0f), yaw(-90.0f), fov(45.0f) {}

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position.z -= 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position.z += 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position.x -= 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position.x += 2.5f * deltaTime;
}

void Camera::apply(GLuint shaderProgram) const {
    glm::mat4 viewMatrix = glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    GLint viewLoc = glGetUniformLocation(shaderProgram, "uniCameraMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
}

float Camera::getFOV() const {
    return fov;
}

glm::vec3 Camera::getPosition() const { return position; }
float Camera::getPitch() const { return pitch; }
float Camera::getYaw() const { return yaw; }

} 
