#include "Skybox.h"

#include "../AssetManager/AssetManager.h"


Skybox::Skybox() {
  shader = AssetManager::instance().loadShaderProgram("assets/shaders/skybox");
}

void Skybox::update(const glm::mat4& projection, const glm::mat4& cameraView, float deltaTime) {
  rotation += rotationSpeed * deltaTime;
  transform = projection * glm::mat4(glm::mat3(cameraView));
}

void Skybox::render() {
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);

  shader->bind();
  shader->setMat4("transform", transform * glm::rotate(rotation, glm::vec3(1, 0, 0)));
  vertexArray.renderIndexed();

  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
}
