#include "Skybox.h"

#include "../AssetManager/AssetManager.h"


Skybox::Skybox() {
  cubeMap = AssetManager::instance().loadCubeMap(
     "assets/textures/skybox/empty.png;"
     "assets/textures/skybox/empty.png;"
     "assets/textures/skybox/sun.png;"
     "assets/textures/skybox/moon.png;"
     "assets/textures/skybox/empty.png;"
     "assets/textures/skybox/empty.png");
  shader = AssetManager::instance().loadShaderProgram("assets/shaders/skybox");
}

void Skybox::update(const glm::mat4& projection, const glm::mat4& cameraView, float deltaTime) {
  rotation += rotationSpeed * deltaTime;
  transform = projection * glm::mat4(glm::mat3(cameraView));
}

void Skybox::render() {
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  shader->bind();
  shader->setTexture("cubeMap", cubeMap, 1);
  shader->setMat4("transform", transform * glm::rotate(rotation, glm::vec3(1, 0, 0)));
  vertexArray.renderIndexed();

  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
}
