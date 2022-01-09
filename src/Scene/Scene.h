#pragma once

#include <utility>

#include "../Rendering/ShaderProgram.h"
#include "../Rendering/Texture.h"
#include "../Rendering/VertexArray.h"
#include "../World/World.h"
#include "../glCraft.h"
#include "Crosshair.h"
#include "Player.h"
#include "Skybox.h"

class Scene {
  bool initialized = false;
  bool isMenuOpen = false;

  Ref<World> world = std::make_shared<World>();

  Ref<const ShaderProgram> outlinedBlockShader;
  Ref<VertexArray> outlinedBlockVertexArray;

  Skybox skybox;

  Player player{world};
  glm::mat4 projectionMatrix = glm::mat4(1);

  Crosshair crosshair;

  void toggleMenu();
  void updateMouse();

public:
  Scene();

  void init();
  void update(float deltaTime);

  void render();
  void renderGui();

  void onResized(int32_t width, int32_t height);
  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onCursorPositionEvent(double x, double y);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
};
