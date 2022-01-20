#include "Player.h"

#include "../World/BlockName.h"
#include "../World/RayCasting/Ray.h"

const glm::mat4& Player::updateView() {
  return view = calcView();
}

const glm::mat4& Player::lookAt(glm::vec3 eye, glm::vec3 center) {
  position = eye;
  updatePlayerDirection(center);
  return updateView();
}

const glm::mat4& Player::setPosition(glm::vec3 eye) {
  position = eye;
  return updateView();
}

void Player::update(float deltaTime) {
  gravity += glm::vec3(0, -1, 0) * GravityConstant * deltaTime;
  auto moveDirection = glm::vec3(0);

  for (const auto& [isMoving, direction]: directions) {
    if (!isMoving) {
      continue;
    }

    moveDirection += direction;
  }

  canJump = false;
  glm::vec3 movement(0);
  if (glm::length(moveDirection) > 0) {
    float movementSpeed = isRunning ? runningSpeed : walkingSpeed;
    movement = glm::normalize(moveDirection) * movementSpeed * deltaTime;
  }

  if (isSurvivalMovement) {
    glm::vec3 nextPosition = position + movement;
    if (MovementSimulation::canMove(position, nextPosition, *world)) {
      position = nextPosition;
    }

    nextPosition = position + gravity * deltaTime;
    if (MovementSimulation::canMove(position, nextPosition, *world)) {
      position = nextPosition;
    } else {
      canJump = true;
      gravity = glm::vec3(0);
    }

  } else {
    position += movement;
  }

  updateView();
}


void Player::onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  if (action == 2) {
    return;  // don't respond to repeatedly pressed buttons
  }

  if (key == 87 || key == 265) {  // forward
    directions[0].isMoving = action == 1;
  } else if (key == 83 || key == 264) {  // backward
    directions[1].isMoving = action == 1;
  } else if (key == 65 || key == 263) {  // left
    directions[2].isMoving = action == 1;
  } else if (key == 68 || key == 262) {  // right
    directions[3].isMoving = action == 1;
  } else if (key == 32) {  // space
    if (isSurvivalMovement) {
      if (canJump && action == 1) {
        gravity = glm::vec3(0, GravityConstant / 4, 0);
      }
    } else {
      directions[4].isMoving = action == 1;
    }
  } else if (key == 340) {  // shift
    if (isSurvivalMovement) {
    } else {
      directions[5].isMoving = action == 1;
    }
  } else if (key == 341) {  // ctrl
    isRunning = action == 1;
  }
}

void Player::onMouseButtonEvent(int32_t button, int32_t action, int32_t mods) {
  if (action != 1)
    return;  // ignore the input on mouse button release

  if (button == 0) {  // left click
    if (Ray ray{position, lookDirection, *world, reach}) {
      world->placeBlock(BlockData::BlockType::air, ray.getHitTarget().position);
    }
  } else if (button == 1) {  // right click
    Ray ray{position, lookDirection, *world, reach};
    if (ray && ray.getHitTarget().hasNeighbor) {
      world->placeBlock(blockToPlace, ray.getHitTarget().neighbor);
    }
  } else if (button == 2) {  // middle click
    if (Ray ray{position, lookDirection, *world, reach}) {
      blockToPlace = ray.getHitTarget().block.type;
    }
  }
}

void Player::onCursorPositionEvent(double x, double y) {
  static double lastX = x;
  static double lastY = y;

  if (resetMouse) {
    resetMouse = false;
    lastX = x;
    lastY = y;
  }

  yaw += static_cast<float>(-lastX + x) * mouseSensitivity;
  pitch = glm::clamp(pitch + static_cast<float>(lastY - y) * mouseSensitivity, -89.0f, 89.0f);

  lastX = x;
  lastY = y;
  updatePlayerOrientation();
}

void Player::updatePlayerDirection(glm::vec3 newForward) {
  lookDirection = newForward;
  newForward.y = 0;
  forward = glm::normalize(newForward);
  backward = -newForward;

  right = glm::normalize(glm::cross(newForward, up));
  left = -right;
}

void Player::updatePlayerOrientation() {
  updatePlayerDirection(glm::normalize(glm::vec3{
     glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
     glm::sin(glm::radians(pitch)),
     glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
  }));

  updateView();
}

glm::mat4 Player::calcView() {
  return glm::lookAt(position, position + lookDirection, up);
}

glm::vec3 Player::getPosition() {
  return position;
}

glm::vec3 Player::getLookDirection() {
  return lookDirection;
}
void Player::resetMousePosition() {
  resetMouse = true;
}
