#pragma once

#include <utility>

#include "../Persistence/Persistence.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/Texture.h"
#include "../Rendering/VertexArray.h"
#include "../World/World.h"
#include "../glCraft.h"
#include "BlockOutline.h"
#include "Crosshair.h"
#include "Player.h"
#include "PostProcessEffect.h"
#include "Skybox.h"

class Scene {
  Ref<Persistence> persistence;
  Ref<World> world;

  Skybox skybox;

  Player player;

  const float zNear = 0.1f;
  const float zFar = 350.0f;
  glm::mat4 projectionMatrix = glm::mat4(1);

  Crosshair crosshair;
  BlockOutline outline;
  PostProcessEffect vignetteEffect;
  PostProcessEffect invertEffect;

  bool isMenuOpen = false;
  bool showIntermediateTextures = false;
  bool enableXRay = false;
  bool enableVignetteEffect = true;
  bool enableInvertEffect = false;
  float vignetteIntensity = 2.9;
  float vignetteStart = 1.5f;

  void toggleMenu();
  void updateMouse();

public:
  explicit Scene(const std::string& savePath);

  void update(float deltaTime);

  void render();
  void renderGui();
  void renderMenu();
  void renderIntermediateTextures();

  void onResized(int32_t width, int32_t height);
  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onCursorPositionEvent(double x, double y);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
};
