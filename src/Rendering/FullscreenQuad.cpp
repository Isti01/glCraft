#include "FullscreenQuad.h"

Ref<VertexArray> FullscreenQuad::getVertexArray() {
  static Ref<VertexArray> vertexArray =
     std::make_shared<VertexArray>(std::vector<glm::vec3>{{-1, -1, 0}, {-1, 1, 0}, {1, -1, 0}, {1, 1, 0}},
                                   std::vector<VertexAttribute>{VertexAttribute{3, VertexAttribute::Float, 0}},
                                   std::vector<uint8_t>{0, 2, 1, 1, 2, 3});
  return vertexArray;
}
