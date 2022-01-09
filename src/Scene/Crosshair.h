#pragma once

#include "../Rendering/ShaderProgram.h"
#include "../Rendering/VertexArray.h"
#include "../glCraft.h"

class Crosshair {
  glm::mat4 transform;
  VertexArray vertexArray;
  Ref<const ShaderProgram> shader;

public:
  Crosshair();

  void update(float aspectRatio);
  void render();
};