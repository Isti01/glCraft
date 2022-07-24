#include "Scene.h"

#include "../Application/Application.h"
#include "../Math/WorldRayCast.h"
#include "../World/BlockName.h"

Scene::Scene(const std::string& savePath)
    : persistence(std::make_shared<Persistence>(savePath)),
      world(std::make_shared<World>(persistence)),
      player(world, persistence),
      vignetteEffect(AssetManager::instance().loadShaderProgram("assets/shaders/vignette_effect")),
      invertEffect(AssetManager::instance().loadShaderProgram("assets/shaders/invert_effect")) {
  onResized(Application::instance().getWindowWidth(), Application::instance().getWindowHeight());
  updateMouse();
}

void Scene::update(float deltaTime) {
  player.update(deltaTime);
  world->update(player.getCamera().getPosition(), deltaTime);
  skybox.update(projectionMatrix, player.getCamera().getViewMatrix(), deltaTime);
}

void Scene::toggleMenu() {
  isMenuOpen = !isMenuOpen;
  updateMouse();
}

void Scene::updateMouse() {
  if (isMenuOpen) {
    player.resetMousePosition();
    Window::instance().unlockMouse();
  } else {
    Window::instance().lockMouse();
  }
}

void Scene::render() {
  skybox.render();

  const glm::mat4 mvp = projectionMatrix * player.getCamera().getViewMatrix();
  const Camera& camera = player.getCamera();
  if (enableXRay) {
    const int32_t width = Window::instance().getWindowWidth();
    const int32_t height = Window::instance().getWindowHeight();
    world->renderTransparent(mvp, zNear, zFar, width, height);
  } else {
    world->renderOpaque(camera.getPosition(), mvp);
  }

  if (WorldRayCast ray{camera.getPosition(), camera.getLookDirection(), *world, Player::Reach}) {
    outline.render(mvp * glm::translate(ray.getHitTarget().position));
  }

  crosshair.render();
  if (enableInvertEffect) {
    invertEffect.render();
  }
  if (enableVignetteEffect) {
    vignetteEffect.getShader()->setFloat("intensity", vignetteIntensity);
    vignetteEffect.getShader()->setFloat("start", vignetteStart);
    vignetteEffect.render();
  }
}

void Scene::renderMenu() {
  if (ImGui::Begin("Menu")) {
    glm::vec3 position = player.getCamera().getPosition();
    ImGui::Text("Player position: x:%f, y:%f, z:%f", position.x, position.y, position.z);
    glm::vec3 lookDirection = player.getCamera().getLookDirection();
    ImGui::Text("Player direction: x:%f, y:%f, z:%f", lookDirection.x, lookDirection.y, lookDirection.z);

    ImGui::Spacing();
    ImGui::Spacing();

    bool isSurvival = player.getIsSurvivalMovement();
    if (ImGui::Checkbox("Enable \"physics\"", &isSurvival)) {
      player.setSurvivalMovement(isSurvival);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Checkbox("Show intermediate textures", &showIntermediateTextures)) {
      Window::instance().getFramebufferStack()->setKeepIntermediateTextures(showIntermediateTextures);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Checkbox("Enable XRay", &enableXRay);

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Checkbox("Enable invert effect", &enableInvertEffect);

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Checkbox("Enable vignette effect", &enableVignetteEffect);

    if (enableVignetteEffect) {
      float invertedIntensity = 4 - vignetteIntensity;
      if (ImGui::SliderFloat("Vignette intensity", &invertedIntensity, 1, 3)) {
        vignetteIntensity = 4 - invertedIntensity;
      }

      ImGui::SliderFloat("Vignette start", &vignetteStart, 0, 3);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    BlockData::BlockType blockToPlace = player.getBlockToPlace();
    ImGui::Text("Selected Block: %s", BlockName::blockTypeToName(blockToPlace));

    ImGui::Spacing();
    ImGui::Spacing();

    BlockName::NameArray names = BlockName::getBlockNames();
    int32_t selected = BlockName::blockTypeToIndex(blockToPlace);
    if (ImGui::ListBox("Select a block to place", &selected, &names[0], names.size())) {
      player.setBlockToPlace(BlockName::BlockNames[selected].first);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    int32_t useOcclusion = world->getUseAmbientOcclusion() ? 1 : 0;
    if (ImGui::SliderInt("Use ambient occlusion", &useOcclusion, 0, 1)) {
      world->setUseAmbientOcclusion(useOcclusion == 1);
    }

    ImGui::Spacing();

    int32_t distance = world->getViewDistance();
    if (ImGui::SliderInt("Max render distance", &distance, 1, 13)) {
      world->setViewDistance(distance);
    }

    ImGui::Spacing();

    float speed = skybox.getRotationSpeed();
    if (ImGui::SliderFloat("Night/Day cycle speed", &speed, 0, 10)) {
      skybox.setRotationSpeed(speed);
    }

    ImGui::Spacing();

    float movementSpeed = player.getMovementSpeedMultiplier();
    if (ImGui::SliderFloat("Player movement speed multiplier", &movementSpeed, 1.0f, 10.0f)) {
      player.setMovementSpeedMultiplier(movementSpeed);
    }

    ImGui::Spacing();

    float jumpHeight = player.getJumpHeightMultiplier();
    if (ImGui::SliderFloat("Player jump height multiplier", &jumpHeight, 1.0f, 10.0f)) {
      player.setJumpHeightMultiplier(jumpHeight);
    }

    ImGui::Spacing();

    float gravity = player.getGravityConstant() / 10;
    if (ImGui::SliderFloat("Gravity", &gravity, -5, 10.0f)) {
      player.setGravityConstant(gravity * 10);
    }

    ImGui::Spacing();

    if (ImGui::Button("Reset gravity")) {
      player.setGravityConstant(Player::DefaultGravity);
    }

    ImGui::Spacing();
    ImGui::Spacing();
    {
      const uint32_t pathLength = 256;
      static char textureAtlasPath[pathLength] = "";
      ImGui::InputText("Custom texture atlas path", textureAtlasPath, pathLength);
      if (ImGui::Button("Load texture atlas")) {
        Ref<const Texture> atlas = AssetManager::instance().loadTexture(textureAtlasPath);
        if (atlas != nullptr) {
          world->setTextureAtlas(atlas);
        }
      }
    }

    ImGui::Spacing();
    ImGui::Spacing();
    {
      const uint32_t pathLength = 256;
      static char textureAtlasPath[pathLength] = "";
      ImGui::InputText("Save file path", textureAtlasPath, pathLength);
      if (ImGui::Button("Load World")) {
        if (std::filesystem::exists(textureAtlasPath)) {
          Application::instance().setScene(std::make_shared<Scene>(textureAtlasPath));
        }
      }
    }
  }

  ImGui::End();
}

void Scene::renderIntermediateTextures() {
  if (ImGui::Begin("Intermediate Textures")) {
    for (const auto& texture: Window::instance().getFramebufferStack()->getIntermediateTextures()) {
      ImGui::Text("%u", texture->getId());
      ImGui::Image(reinterpret_cast<ImTextureID>(texture->getId()), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
    }
  }
  ImGui::End();
}

void Scene::renderGui() {
  if (showIntermediateTextures) {
    renderIntermediateTextures();
  }

  if (isMenuOpen) {
    renderMenu();
  }
}

void Scene::onResized(int32_t width, int32_t height) {
  float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);
  projectionMatrix = glm::perspective<float>(glm::half_pi<float>(), aspectRatio, zNear, zFar);
  crosshair.update(aspectRatio);
}

void Scene::onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  if (key == GLFW_KEY_ESCAPE) {
    if (action == GLFW_PRESS) {
      toggleMenu();
    }
    return;
  }
  if (!isMenuOpen) {
    player.onKeyEvent(key, scancode, action, mode);
  }
}

void Scene::onMouseButtonEvent(int32_t button, int32_t action, int32_t mods) {
  if (!isMenuOpen) {
    player.onMouseButtonEvent(button, action, mods);
  }
}

void Scene::onCursorPositionEvent(double x, double y) {
  if (!isMenuOpen) {
    player.onCursorPositionEvent(x, y);
  }
}
