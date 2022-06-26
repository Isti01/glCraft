#include "Player.h"

#include "../Math/WorldRayCast.h"

Player::Player(const Ref<World>& world, const Ref<Persistence>& persistence)
    : camera(persistence->getCamera()),
      persistence(persistence),
      world(world) {}

Player::~Player() {
  persistence->commitCamera(camera);
}

void Player::update(float deltaTime) {
  gravity += glm::vec3(0, -1, 0) * gravityConstant * deltaTime;

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
  if (action == GLFW_REPEAT) {
    return;  // don't respond to repeatedly pressed buttons
  }

  bool isButtonPressed = action == GLFW_PRESS;

  if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
    camera.setIsMovingForward(isButtonPressed);
  } else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
    camera.setIsMovingBackward(isButtonPressed);
  } else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
    camera.setIsMovingLeft(isButtonPressed);
  } else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
    camera.setIsMovingRight(isButtonPressed);
  } else if (key == GLFW_KEY_SPACE) {
    if (isSurvivalMovement) {
      camera.setIsMovingUp(false);
      if (canJump && isButtonPressed) {
        gravity = glm::vec3(0, getJumpSpeed(), 0);
      }
    } else {
      camera.setIsMovingUp(isButtonPressed);
    }
  } else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
    if (isSurvivalMovement) {
      camera.setIsMovingDown(false);
    } else {
      camera.setIsMovingDown(isButtonPressed);
    }
  } else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
    isRunning = isButtonPressed;
  }
}

void Player::onMouseButtonEvent(int32_t button, int32_t action, int32_t) {
  if (action != GLFW_PRESS) {
    return;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (WorldRayCast ray{camera.getPosition(), camera.getLookDirection(), *world, Reach}) {
      world->placeBlock(BlockData::BlockType::air, ray.getHitTarget().position);
    }
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    WorldRayCast ray{camera.getPosition(), camera.getLookDirection(), *world, Reach};
    if (ray && ray.getHitTarget().hasNeighbor) {
      world->placeBlock(blockToPlace, ray.getHitTarget().neighbor);
    }
  } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
    if (WorldRayCast ray{camera.getPosition(), camera.getLookDirection(), *world, Reach}) {
      blockToPlace = ray.getHitTarget().block.type;
    }
  }
}

void Player::onCursorPositionEvent(double x, double y) {
  static double lastX = x;
  static double lastY = y;

  if (shouldResetMouse) {
    shouldResetMouse = false;
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
  shouldResetMouse = true;
}
