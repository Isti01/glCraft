#include "Player.h"

#include "../Math/Ray.h"

Player::Player(const Ref<World>& world, const Ref<Persistence>& persistence)
    : camera(persistence->getCamera()),
      persistence(persistence),
      world(world) {}

void Player::update(float deltaTime) {
  gravity += glm::vec3(0, -1, 0) * GravityConstant * deltaTime;

  glm::vec3 moveDirection = camera.getMoveDirection();

  canJump = false;
  glm::vec3 movement(0);
  if (glm::length(moveDirection) > 0) {
    float movementSpeed = isRunning ? getRunningSpeed() : getWalkingSpeed();
    movement = glm::normalize(moveDirection) * movementSpeed * deltaTime;
  }

  glm::vec3 position = camera.getPosition();

  if (isSurvivalMovement) {
    std::array<glm::vec3, 3> axes = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};

    for (const auto& axis: axes) {
      glm::vec3 movementInAxis = movement * axis;
      if (MovementSimulation::canMove(position, position + movementInAxis, *world)) {
        position += movementInAxis;
      }
    }

    glm::vec3 positionWithGravity = position + gravity * deltaTime;
    if (MovementSimulation::canMove(position, positionWithGravity, *world)) {
      position = positionWithGravity;
    } else {
      canJump = true;
      gravity = glm::vec3(0);
    }

  } else {
    position += movement;
  }

  camera.setPosition(position);
}

void Player::onKeyEvent(int32_t key, int32_t, int32_t action, int32_t) {
  if (action == 2) {
    return;  // don't respond to repeatedly pressed buttons
  }

  bool isButtonPressed = action == 1;

  if (key == 87 || key == 265) {  // forward
    camera.setIsMovingForward(isButtonPressed);
  } else if (key == 83 || key == 264) {  // backward
    camera.setIsMovingBackward(isButtonPressed);
  } else if (key == 65 || key == 263) {  // left
    camera.setIsMovingLeft(isButtonPressed);
  } else if (key == 68 || key == 262) {  // right
    camera.setIsMovingRight(isButtonPressed);
  } else if (key == 32) {  // space
    if (isSurvivalMovement) {
      camera.setIsMovingUp(false);
      if (canJump && isButtonPressed) {
        gravity = glm::vec3(0, GravityConstant / 4.5, 0);
      }
    } else {
      camera.setIsMovingUp(isButtonPressed);
    }
  } else if (key == 340) {  // shift
    if (isSurvivalMovement) {
      camera.setIsMovingDown(false);
    } else {
      camera.setIsMovingDown(isButtonPressed);
    }
  } else if (key == 341) {  // ctrl
    isRunning = isButtonPressed;
  }
}

void Player::onMouseButtonEvent(int32_t button, int32_t action, int32_t) {
  if (action != 1)
    return;  // ignore the input on mouse button release

  if (button == 0) {  // left click
    if (Ray ray{camera.getPosition(), camera.getLookDirection(), *world, reach}) {
      world->placeBlock(BlockData::BlockType::air, ray.getHitTarget().position);
    }
  } else if (button == 1) {  // right click
    Ray ray{camera.getPosition(), camera.getLookDirection(), *world, reach};
    if (ray && ray.getHitTarget().hasNeighbor) {
      world->placeBlock(blockToPlace, ray.getHitTarget().neighbor);
    }
  } else if (button == 2) {  // middle click
    if (Ray ray{camera.getPosition(), camera.getLookDirection(), *world, reach}) {
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

  float yaw = camera.getYaw() + static_cast<float>(-lastX + x) * mouseSensitivity;
  float pitch = glm::clamp(camera.getPitch() + static_cast<float>(lastY - y) * mouseSensitivity, -89.0f, 89.0f);
  camera.updateCameraOrientation(yaw, pitch);

  lastX = x;
  lastY = y;
}

void Player::resetMousePosition() {
  resetMouse = true;
}

Player::~Player() {
  persistence->commitCamera(camera);
}
