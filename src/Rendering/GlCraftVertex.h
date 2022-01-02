#pragma once

#include "../glCraft.h"

struct GlCraftVertex {
  glm::vec3 position;
  glm::vec2 uvCoords;

  static std::vector<VertexAttribute> vertexAttributes() {
    return {VertexAttribute(3, VertexAttribute::Float, 0),
            VertexAttribute(2, VertexAttribute::Float, sizeof(glm::vec3))};
  }
};