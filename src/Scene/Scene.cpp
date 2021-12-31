#include "Scene.h"

#include "../AssetManager/AssetManager.h"

void Scene::init() {
  if (initialized) throw std::exception("The scene has been already initialized");
  initialized = true;

  for (auto &entity: entities) { entity.init(); }

  std::vector<glm::vec3> vertices = {{0, .5, 0}, {.5, -.5, 0}, {-.5, -.5, 0}};
  std::vector<VertexAttribute> vertexAttributes = {VertexAttribute(3, VertexAttribute::Float, 0)};
  std::vector<uint32_t> indices = {0, 1, 2};

  vao = std::make_shared<VertexArray>(vertices, vertexAttributes, indices);
  defaultShader = AssetManager::instance().loadShaderProgram("default");
}

void Scene::update() {
  for (auto &entity: entities) { entity.update(); }
}

void Scene::render() {
  defaultShader->bind();
  vao->render();
}
void Scene::renderGui() {
  ImGui::ShowDemoWindow();
}
