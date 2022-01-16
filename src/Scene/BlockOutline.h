#pragma once

#include "../Rendering/ShaderProgram.h"
#include "../Rendering/VertexArray.h"
#include "../glCraft.h"

class BlockOutline {
  Ref<const ShaderProgram> outlinedBlockShader;
  Ref<VertexArray> outlinedBlockVertexArray;

public:
  BlockOutline();

  void render(const glm::mat4& transform);
};