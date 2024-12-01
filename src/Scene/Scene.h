#pragma once

#include <utility>

#include "../Persistence/Persistence.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/Texture.h"
#include "../Rendering/VertexArray.h"
#include "../World/World.h"
#include "../glCraft.h"
#include "BlockOutline.h"
#include "Effects/ChromaticAberrationEffect.h"
#include "Effects/CrosshairEffect.h"
#include "Effects/GammaCorrectionEffect.h"
#include "Effects/GaussianBlurEffect.h"
#include "Effects/InvertEffect.h"
#include "Effects/PostProcessEffect.h"
#include "Effects/VignetteEffect.h"
#include "Player.h"
#include "Skybox.h"

class Scene {
  Ref<Persistence> persistence;
  Ref<World> world;

  Skybox skybox;
  Player player;

  const float zNear = 0.1f;
  const float zFar = 1000.0f;
  glm::mat4 projectionMatrix = glm::mat4(1);
  float deltaTime = 1.0f;
  BlockOutline outline{std::make_shared<CubeMesh>()};
  std::vector<Ref<PostProcessEffect>> postProcessingEffects = {
     std::make_shared<CrosshairEffect>(true),       std::make_shared<ChromaticAberrationEffect>(false),
     std::make_shared<InvertEffect>(false),         std::make_shared<VignetteEffect>(true),
     std::make_shared<GammaCorrectionEffect>(true), std::make_shared<GaussianBlurEffect>(false)};

  bool isMenuOpen = false;
  bool showIntermediateTextures = false;

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
