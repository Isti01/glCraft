#pragma once

#include "../Physics/MovementSimulation.h"
#include "../World/World.h"
#include "../glCraft.h"
#include "Camera.h"

class Player {
  static constexpr float GravityConstant = 46.62f;

  Camera camera;

  Ref<World> world;
  BlockData::BlockType blockToPlace = BlockData::BlockType::grass;


  glm::vec3 gravity{0};


  float walkingSpeed = 4.317;
  float runningSpeed = 5.612;
  float mouseSensitivity = .5;
  bool canJump = false;
  bool isRunning = false;
  bool isSurvivalMovement = false;
  bool resetMouse = true;

public:
  static constexpr float reach = 4.5f;

  explicit Player(const Ref<World>& world) : world(world) {}

  void update(float deltaTime);

  [[nodiscard]] bool getIsSurvivalMovement() const { return isSurvivalMovement; };
  void setSurvivalMovement(bool isSurvival) {
    gravity = glm::vec3(0);
    isSurvivalMovement = isSurvival;
  };

  [[nodiscard]] const Camera& getCamera() const { return camera; };

  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
  void onCursorPositionEvent(double d, double d1);

  [[nodiscard]] BlockData::BlockType getBlockToPlace() const { return blockToPlace; };
  void setBlockToPlace(BlockData::BlockType block) { blockToPlace = block; };

  void resetMousePosition();
};
