#include "Scene.h"

#include "../Application/Application.h"
#include "../AssetManager/AssetManager.h"
#include "../Rendering/BlockVertex.h"
#include "Chunk.h"

Scene::Scene() {
  onResized(Application::instance().getWindowWidth(), Application::instance().getWindowHeight());
}

Scene::Scene(std::vector<Entity> entities) : entities(std::move(entities)) {
  onResized(Application::instance().getWindowWidth(), Application::instance().getWindowHeight());
}

void Scene::init() {
  if (initialized) throw std::exception("The scene has been already initialized");
  initialized = true;

  for (auto &entity: entities) { entity.init(); }

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
  static float animationProgress = 0;
  animationProgress += deltaTime * 2;

  for (auto &entity: entities) { entity.update(deltaTime); }

  camera.setPosition({
     glm::sin(animationProgress) * 5,
     3,
     glm::cos(animationProgress) * 5,
  });
}

void Scene::render() {
  glm::mat4 mvp = projectionMatrix * camera.getViewMatrix();


  chunk->render(mvp);
}

void Scene::renderGui() {
  ImGui::ShowDemoWindow();
}

void Scene::onResized(int32_t width, int32_t height) {
  float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);
  projectionMatrix = glm::perspective<float>(glm::half_pi<float>(), aspectRatio, .1f, 100.0f);
}
