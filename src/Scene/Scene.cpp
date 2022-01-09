#include "Scene.h"

#include "../Application/Application.h"
#include "../AssetManager/AssetManager.h"
#include "../World/RayCasting/Ray.h"
#include "../World/World.h"

Scene::Scene() {
  onResized(Application::instance().getWindowWidth(), Application::instance().getWindowHeight());
}

void Scene::init() {
  if (initialized) throw std::exception("The scene has been already initialized");
  initialized = true;

  updateMouse();

  outlinedBlockShader = AssetManager::instance().loadShaderProgram("assets/shaders/outline");

  std::vector<BlockVertex> vertices;

  vertices.resize(6 * 6);
  int vertexCount = 0;
  for (const auto& face: BlockMesh::vertices) {
    for (const auto& vertex: face) {
      vertices.at(vertexCount) = vertex;
      vertexCount++;
    }
  }
  outlinedBlockVertexArray = std::make_shared<VertexArray>(vertices, BlockVertex::vertexAttributes());

  world = std::make_shared<World>();

  for (int x = -25; x < 25; ++x) {
    for (int z = -25; z < 25; ++z) {
      world->placeBlock(BlockData::BlockType::grass, {x, 3, z});
      world->placeBlock(BlockData::BlockType::dirt, {x, 2, z});
      world->placeBlock(BlockData::BlockType::dirt, {x, 1, z});
      world->placeBlock(BlockData::BlockType::stone, {x, 0, z});
    }
  }
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
    player.resetMousePosition();
    Window::instance().unlockMouse();
  } else {
    Window::instance().lockMouse();
  }
}

void Scene::render() {
  glm::mat4 mvp = projectionMatrix * player.getViewMatrix();
  world->render(player.getPosition(), mvp);


  // render the block outline
  Ray ray(player.getPosition(), player.getLookDirection(), *world, 10.0f);
  if (ray.hasHit()) {
    auto blockHit = ray.getHitTarget().position;

    outlinedBlockShader->setMat4("MVP", mvp * glm::translate(blockHit));
    outlinedBlockShader->bind();
    outlinedBlockVertexArray->renderVertexStream();
  }

  crosshair.render();
}

void Scene::renderGui() {
  if (!isMenuOpen) return;

  ImGui::Begin("Place Block");
  static float coords[] = {0, 0, 0};

  if (ImGui::SliderFloat3("Block Coordinate: ", &coords[0], -20, 20)) {
    world->placeBlock(BlockData::BlockType::cobble_stone, {coords[0], coords[1], coords[2]});
  }

  ImGui::Text("Player position: x:%f, y:%f, z:%f", player.getPosition().x, player.getPosition().y,
              player.getPosition().z);

  ImGui::Text("Player direction: x:%f, y:%f, z:%f", player.getLookDirection().x, player.getLookDirection().y,
              player.getLookDirection().z);

  ImGui::End();
}

void Scene::onResized(int32_t width, int32_t height) {
  float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);
  projectionMatrix = glm::perspective<float>(glm::half_pi<float>(), aspectRatio, .1f, 250.0f);
  crosshair.update(aspectRatio);
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
