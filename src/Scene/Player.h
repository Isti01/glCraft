#pragma once

#include "../Physics/MovementSimulation.h"
#include "../World/World.h"
#include "../glCraft.h"
#include "Camera.h"

class Player {
  Camera camera;

  Ref<World> world;
  Ref<Persistence> persistence;
  BlockData::BlockType blockToPlace = BlockData::BlockType::grass;

  glm::vec3 gravity{0};

  float movementSpeedMultiplier = 1;
  float mouseSensitivity = .5;

  float gravityConstant = DefaultGravity;
  float jumpHeightMultiplier = 1;
  bool canJump = false;

  bool isRunning = false;
  bool isSurvivalMovement = false;
  bool shouldResetMouse = true;

public:
  static constexpr float DefaultGravity = 46.62f;
  static constexpr float Reach = 4.5f;

  explicit Player(const Ref<World>& world, const Ref<Persistence>& persistence);
  ~Player();

  void update(float deltaTime);

  [[nodiscard]] bool getIsSurvivalMovement() const { return isSurvivalMovement; };
  void setSurvivalMovement(bool isSurvival) {
    gravity = glm::vec3(0);
    isSurvivalMovement = isSurvival;
  };

  [[nodiscard]] const Camera& getCamera() const { return camera; };

  [[nodiscard]] float getJumpHeightMultiplier() const { return jumpHeightMultiplier; };
  void setJumpHeightMultiplier(float multiplier) { jumpHeightMultiplier = multiplier; };

  [[nodiscard]] float getGravityConstant() const { return gravityConstant; };
  void setGravityConstant(float constant) { gravityConstant = constant; };

  [[nodiscard]] float getMovementSpeedMultiplier() const { return movementSpeedMultiplier; };
  void setMovementSpeedMultiplier(float movementSpeed) { movementSpeedMultiplier = movementSpeed; }

  [[nodiscard]] float getJumpSpeed() const { return jumpHeightMultiplier * gravityConstant / 4.5f; };
  [[nodiscard]] float getWalkingSpeed() const { return movementSpeedMultiplier * 4.317f; };
  [[nodiscard]] float getRunningSpeed() const { return movementSpeedMultiplier * 5.612f; };

  [[nodiscard]] BlockData::BlockType getBlockToPlace() const { return blockToPlace; };
  void setBlockToPlace(BlockData::BlockType block) { blockToPlace = block; };

  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
  void onCursorPositionEvent(double d, double d1);

  void resetMousePosition();
};
