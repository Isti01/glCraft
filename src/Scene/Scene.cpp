#include "Scene.h"

#include "../Application/Application.h"
#include "../AssetManager/AssetManager.h"

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

  std::vector<glm::vec3> vertices = {
     {1, 1, 1},     //
     {1, 1, -1},    //
     {1, -1, 1},    //
     {1, -1, -1},   //
     {-1, 1, 1},    //
     {-1, 1, -1},   //
     {-1, -1, 1},   //
     {-1, -1, -1},  // don't format
  };
  std::vector<VertexAttribute> vertexAttributes = {VertexAttribute(3, VertexAttribute::Float, 0)};
  std::vector<uint32_t> indices = {
     // top
     0, 1, 5,  //
     4, 0, 5,  //
     // +x east
     2, 1, 0,  //
     1, 2, 3,  //
     // -x west
     7, 4, 5,  //
     4, 7, 6,  //
     // -z north
     1, 3, 7,  //
     1, 7, 5,  //
     // +z south
     4, 6, 0,  //
     2, 0, 6,  //
     // bottom
     3, 2, 6,  //
     7, 3, 6   //
  };

  vao = std::make_shared<VertexArray>(vertices, vertexAttributes, indices);
  defaultShader = AssetManager::instance().loadShaderProgram("assets/default");
}

void Scene::update(float deltaTime) {
  static float animationProgress = 0;
  animationProgress += deltaTime * 2;

  for (auto &entity: entities) { entity.update(deltaTime); }

  camera.setPosition({sin(animationProgress) * 5, sin(animationProgress / 2) * 3, cos(animationProgress) * 5});
}

void Scene::render() {
  glm::mat4 mvp = projectionMatrix * camera.getViewMatrix();
  defaultShader->bind();
  defaultShader->setMat4("MVP", mvp);

  vao->render();
}
void Scene::renderGui() {
  ImGui::ShowDemoWindow();
}
void Scene::onResized(int32_t width, int32_t height) {
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  projectionMatrix = glm::perspective<float>(glm::half_pi<float>(), aspectRatio, .1f, 100.0f);
}
