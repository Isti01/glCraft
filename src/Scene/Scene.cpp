#include "Scene.h"

#include "../Application/Application.h"
#include "../AssetManager/AssetManager.h"
#include "Chunk.h"

Scene::Scene() {
  onResized(Application::instance().getWindowWidth(), Application::instance().getWindowHeight());
}

void Scene::init() {
  if (initialized) throw std::exception("The scene has been already initialized");
  initialized = true;

  updateMouse();

  chunk = std::make_shared<Chunk>(glm::ivec2(0, 0));

  chunk->placeBlock(BlockData::BlockType::grass, {0, 2, 0});
  chunk->placeBlock(BlockData::BlockType::dirt, {0, 1, 0});
  chunk->placeBlock(BlockData::BlockType::stone, {0, 0, 0});
  chunk->placeBlock(BlockData::BlockType::grass, {1, 2, 0});
  chunk->placeBlock(BlockData::BlockType::dirt, {1, 1, 0});
  chunk->placeBlock(BlockData::BlockType::stone, {1, 0, 0});
  chunk->placeBlock(BlockData::BlockType::grass, {2, 2, 0});
  chunk->placeBlock(BlockData::BlockType::dirt, {2, 1, 0});
  chunk->placeBlock(BlockData::BlockType::stone, {2, 0, 0});

  chunk->placeBlock(BlockData::BlockType::grass, {0, 2, 1});
  chunk->placeBlock(BlockData::BlockType::dirt, {0, 1, 1});
  chunk->placeBlock(BlockData::BlockType::stone, {0, 0, 1});
  chunk->placeBlock(BlockData::BlockType::grass, {1, 2, 1});
  chunk->placeBlock(BlockData::BlockType::dirt, {1, 1, 1});
  chunk->placeBlock(BlockData::BlockType::stone, {1, 0, 1});
  chunk->placeBlock(BlockData::BlockType::grass, {2, 2, 1});
  chunk->placeBlock(BlockData::BlockType::dirt, {2, 1, 1});
  chunk->placeBlock(BlockData::BlockType::stone, {2, 0, 1});

  chunk->placeBlock(BlockData::BlockType::grass, {0, 2, 2});
  chunk->placeBlock(BlockData::BlockType::dirt, {0, 1, 2});
  chunk->placeBlock(BlockData::BlockType::stone, {0, 0, 2});
  chunk->placeBlock(BlockData::BlockType::grass, {1, 2, 2});
  chunk->placeBlock(BlockData::BlockType::dirt, {1, 1, 2});
  chunk->placeBlock(BlockData::BlockType::stone, {1, 0, 2});
  chunk->placeBlock(BlockData::BlockType::grass, {2, 2, 2});
  chunk->placeBlock(BlockData::BlockType::dirt, {2, 1, 2});
  chunk->placeBlock(BlockData::BlockType::stone, {2, 0, 2});

  defaultShader = AssetManager::instance().loadShaderProgram("assets/shaders/default");
  textureAtlas = AssetManager::instance().loadTexture("assets/textures/default_texture.png");
  defaultShader->setTexture("atlas", textureAtlas, 0);
}

void Scene::update(float deltaTime) {
  player.update(deltaTime);
}

void Scene::toggleMenu() {
  isMenuOpen = !isMenuOpen;
  updateMouse();
}

void Scene::updateMouse() {
  if (isMenuOpen) {
    Window::instance().unlockMouse();
  } else {
    Window::instance().lockMouse();
  }
}

void Scene::render() {
  glm::mat4 mvp = projectionMatrix * player.getViewMatrix();
  chunk->render(mvp);
}

void Scene::renderGui() {
  if (isMenuOpen) { ImGui::ShowDemoWindow(); }
}

void Scene::onResized(int32_t width, int32_t height) {
  float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);
  projectionMatrix = glm::perspective<float>(glm::half_pi<float>(), aspectRatio, .1f, 100.0f);
}

void Scene::onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  if (key == 256) {
    if (action == 1) { toggleMenu(); }
    return;
  }
  player.onKeyEvent(key, scancode, action, mode);
}

void Scene::onMouseButtonEvent(int32_t button, int32_t action, int32_t mods) {
  if (!isMenuOpen) { player.onMouseButtonEvent(button, action, mods); }
}
void Scene::onCursorPositionEvent(double x, double y) {
  if (!isMenuOpen) { player.onCursorPositionEvent(x, y); }
}
