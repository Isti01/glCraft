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

  world->placeBlock(BlockData::BlockType::grass, {0, 2, 0});
  world->placeBlock(BlockData::BlockType::dirt, {0, 1, 0});
  world->placeBlock(BlockData::BlockType::stone, {0, 0, 0});
  world->placeBlock(BlockData::BlockType::grass, {1, 2, 0});
  world->placeBlock(BlockData::BlockType::dirt, {1, 1, 0});
  world->placeBlock(BlockData::BlockType::stone, {1, 0, 0});
  world->placeBlock(BlockData::BlockType::glass, {2, 2, 0});
  world->placeBlock(BlockData::BlockType::dirt, {2, 1, 0});
  world->placeBlock(BlockData::BlockType::stone, {2, 0, 0});

  world->placeBlock(BlockData::BlockType::grass, {0, 2, 1});
  world->placeBlock(BlockData::BlockType::dirt, {0, 1, 1});
  world->placeBlock(BlockData::BlockType::stone, {0, 0, 1});
  world->placeBlock(BlockData::BlockType::grass, {1, 2, 1});
  world->placeBlock(BlockData::BlockType::dirt, {1, 1, 1});
  world->placeBlock(BlockData::BlockType::stone, {1, 0, 1});
  world->placeBlock(BlockData::BlockType::glass, {2, 2, 1});
  world->placeBlock(BlockData::BlockType::dirt, {2, 1, 1});
  world->placeBlock(BlockData::BlockType::stone, {2, 0, 1});

  world->placeBlock(BlockData::BlockType::grass, {0, 2, 2});
  world->placeBlock(BlockData::BlockType::dirt, {0, 1, 2});
  world->placeBlock(BlockData::BlockType::stone, {0, 0, 2});
  world->placeBlock(BlockData::BlockType::grass, {1, 2, 2});
  world->placeBlock(BlockData::BlockType::dirt, {1, 1, 2});
  world->placeBlock(BlockData::BlockType::stone, {1, 0, 2});
  world->placeBlock(BlockData::BlockType::grass, {2, 2, 2});
  world->placeBlock(BlockData::BlockType::glass, {2, 1, 2});
  world->placeBlock(BlockData::BlockType::stone, {2, 0, 2});
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
  world->render(player.getPosition(), mvp);

  Ray ray(player.getPosition(), player.getLookDirection(), *world, 10.0f);

  if (ray.hasHit()) {
    auto blockHit = ray.getHitTarget().position;
    std::cout << "hit pos: " << blockHit.x << " " << blockHit.y << " " << blockHit.z << std::endl;

    outlinedBlockShader->setMat4("MVP", mvp * glm::translate(blockHit));
    outlinedBlockShader->bind();
    outlinedBlockVertexArray->renderVertexStream();
  }
}

void Scene::renderGui() {
  if (!isMenuOpen) return;

  ImGui::Begin("Place Block");
  static float coords[] = {0, 0, 0};

  if (ImGui::SliderFloat3("Block Coordinate: ", &coords[0], 0, 100)) {
    world->placeBlock(BlockData::BlockType::cobble_stone, {coords[0], coords[1], coords[2]});
  }

  ImGui::End();
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
