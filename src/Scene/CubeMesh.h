#pragma once

#include "../Rendering/VertexArray.h"

class CubeMesh {
  Ref<VertexArray> cubeVertexArray;

public:
  CubeMesh();
  void render() const;
};
