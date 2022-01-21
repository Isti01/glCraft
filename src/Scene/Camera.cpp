#include "Camera.h"

const glm::mat4& Camera::updateView() {
  return view = calcView();
}

const glm::mat4& Camera::lookAt(glm::vec3 eye, glm::vec3 center) {
  position = eye;
  updateCameraDirection(center);
  return updateView();
}

const glm::mat4& Camera::setPosition(glm::vec3 eye) {
  position = eye;
  return updateView();
}

void Camera::updateCameraDirection(glm::vec3 newForward) {
  lookDirection = newForward;
  newForward.y = 0;
  forward.direction = glm::normalize(newForward);
  backward.direction = -newForward;

  right.direction = glm::normalize(glm::cross(newForward, cameraUp));
  left.direction = -right.direction;
}

void Camera::updateCameraOrientation(float newYaw, float newPitch) {
  yaw = newYaw;
  pitch = newPitch;
  updateCameraDirection(glm::normalize(glm::vec3{
     glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
     glm::sin(glm::radians(pitch)),
     glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
  }));

  updateView();
}

glm::vec3 Camera::getPosition() const {
  return position;
}

glm::mat4 Camera::calcView() const {
  return glm::lookAt(position, position + lookDirection, cameraUp);
}

glm::vec3 Camera::getLookDirection() const {
  return lookDirection;
}
glm::vec3 Camera::getMoveDirection() {
  auto moveDirection = glm::vec3(0);

  std::array<MovementDirection*, 6> directions = {
     &forward, &backward, &left, &right, &up, &down,
  };

  for (const auto direction: directions) {
    if (!direction->isMoving) {
      continue;
    }

    moveDirection += direction->direction;
  }

  return moveDirection;
}
