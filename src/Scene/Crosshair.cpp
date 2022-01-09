#include "Crosshair.h"

#include "../AssetManager/AssetManager.h"

Crosshair::Crosshair()
    : transform(glm::mat4(1)),
      vertexArray(std::vector<glm::vec3>{{.01, 0, 0}, {-.01, 0, 0}, {0, .01, 0}, {0, -.01, 0}},
                  std::vector<VertexAttribute>{{3, VertexAttribute::Float, 0}}) {
  shader = AssetManager::instance().loadShaderProgram("assets/shaders/crosshair");
}

void Crosshair::update(float aspectRatio) {
  transform = glm::scale<float>(glm::vec3(1, aspectRatio, 1));
}

void Crosshair::render() {
  shader->bind();
  shader->setMat4("transform", transform);
  vertexArray.renderVertexStream(GL_LINES);
}
