#pragma once

#include "../Physics/MovementSimulation.h"
#include "../World/World.h"
#include "../glCraft.h"

struct MovementDirection {
  bool isMoving = false;
  glm::vec3 direction = glm::vec3(0);
};

class Player {
  static constexpr float GravityConstant = 46.62f;
  MovementDirection directions[6] = {
     {false, {1, 0, 0}},  {false, {-1, 0, 0}}, {false, {0, 0, 1}},
     {false, {0, 0, -1}}, {false, {0, 1, 0}},  {false, {0, -1, 0}},
  };
  Ref<World> world;
  BlockData::BlockType blockToPlace = BlockData::BlockType::grass;

  glm::vec3 position = {14, 100, 17};
  glm::vec3 up = {0, 1, 0};
  glm::vec3 gravity{0};

  float yaw = 0;
  float pitch = 0;

  float walkingSpeed = 4.317;
  float runningSpeed = 5.612;
  float mouseSensitivity = .5;
  bool canJump = false;
  bool isRunning = false;
  bool isSurvivalMovement = false;
  bool resetMouse = true;

  glm::vec3& forward = directions[0].direction;
  glm::vec3& backward = directions[1].direction;
  glm::vec3& left = directions[2].direction;
  glm::vec3& right = directions[3].direction;

  glm::vec3 lookDirection = forward;

  glm::mat4 view = calcView();
  glm::mat4 calcView();
  const glm::mat4& updateView();

  void updatePlayerDirection(glm::vec3 newForward);
  void updatePlayerOrientation();

public:
  static constexpr float reach = 4.5f;

  Player(const Ref<World>& world) : world(world) {}
  const glm::mat4& setPosition(glm::vec3 eye);
  const glm::mat4& lookAt(glm::vec3 eye, glm::vec3 center);

  [[nodiscard]] const glm::mat4& getViewMatrix() const { return view; }

  void update(float deltaTime);

  [[nodiscard]] bool getIsSurvivalMovement() const { return isSurvivalMovement; };
  void setSurvivalMovement(bool isSurvival) {
    gravity = glm::vec3(0);
    isSurvivalMovement = isSurvival;
  };

  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
  void onCursorPositionEvent(double d, double d1);
  glm::vec3 getPosition();
  glm::vec3 getLookDirection();

  [[nodiscard]] BlockData::BlockType getBlockToPlace() const { return blockToPlace; };
  void setBlockToPlace(BlockData::BlockType block) { blockToPlace = block; };

  void resetMousePosition();
};
