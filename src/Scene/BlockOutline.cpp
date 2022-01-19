#include "BlockOutline.h"

#include "../AssetManager/AssetManager.h"
#include "../Rendering/BlockMesh.h"
#include "../Rendering/BlockVertex.h"

BlockOutline::BlockOutline() {
  outlinedBlockShader = AssetManager::instance().loadShaderProgram("assets/shaders/outline");

  std::vector<BlockVertex> vertices;

  vertices.resize(6 * 6);
  int32_t vertexCount = 0;
  for (const auto& face: BlockMesh::vertices) {
    for (const auto& vertex: face) {
      vertices.at(vertexCount) = vertex;
      vertexCount++;
    }
  }
  outlinedBlockVertexArray = std::make_shared<VertexArray>(vertices, BlockVertex::vertexAttributes());
}

void BlockOutline::render(const glm::mat4& transform) {
  outlinedBlockShader->setMat4("MVP", transform);
  outlinedBlockShader->bind();
  outlinedBlockVertexArray->renderVertexStream();
}
