#include "Chunk.h"

#include "../AssetManager/AssetManager.h"
#include "../Util/Math.h"
#include "World.h"

Chunk::Chunk(const glm::ivec2& worldPosition) : worldPosition(worldPosition) {
  shader = AssetManager::instance().loadShaderProgram("assets/shaders/default");
}

void Chunk::render(const glm::mat4& transform, const World& world) {
  if (!mesh || renderState != RenderState::ready) {
    createMesh(world);
    renderState = RenderState::ready;
  }

  shader->bind();
  shader->setMat4("MVP", transform * glm::translate(glm::vec3(worldPosition.x, 0, worldPosition.y)));

  mesh->renderVertexSubStream(solidVertexCount, 0);
  mesh->renderVertexSubStream(semiTransparentVertexCount, solidVertexCount);
}

void Chunk::createMesh(const World& world) {
  static Ref<std::vector<BlockVertex>> solidVertices = std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);
  static Ref<std::vector<BlockVertex>> semiTransparentVertices =
     std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);

  solidVertexCount = 0;
  semiTransparentVertexCount = 0;

  // used tuple, because glm::ivec3 cannot be destructured
  const std::array<std::tuple<int32_t, int32_t, int32_t>, 6> offsetsToCheck = {{
     {1, 0, 0},
     {-1, 0, 0},
     {0, 1, 0},
     {0, -1, 0},
     {0, 0, 1},
     {0, 0, -1},
  }};

  for (int32_t x = 0; x < HorizontalSize; x++) {
    for (int32_t y = 0; y < VerticalSize; y++) {
      for (int32_t z = 0; z < HorizontalSize; z++) {
        const auto& [type, blockClass] = data[x][y][z];
        if (blockClass == BlockData::BlockClass::air) {
          continue;
        }

        for (const auto& [ox, oy, oz]: offsetsToCheck) {
          int32_t nx = x + ox;
          int32_t ny = y + oy;
          int32_t nz = z + oz;

          if (ny >= 0 && ny < VerticalSize) {
            if (nx >= 0 && nx < HorizontalSize && nz >= 0 && nz < HorizontalSize) {
              if (blockClass == data[nx][ny][nz].blockClass) {
                continue;
              }
            } else if (std::optional<BlockData> block =
                          world.getBlockAtIfLoaded(glm::ivec3(nx + worldPosition.x, ny, nz + worldPosition.y))) {
              if (blockClass == block->blockClass) {
                continue;
              }
            }
          }

          for (const auto& vertex: BlockMesh::getVerticesFromDirection(ox, oy, oz)) {
            if (blockClass == BlockData::BlockClass::semiTransparent ||
                blockClass == BlockData::BlockClass::transparent) {
              semiTransparentVertices->at(semiTransparentVertexCount) = vertex;
              semiTransparentVertices->at(semiTransparentVertexCount).offset(x, y, z);
              semiTransparentVertices->at(semiTransparentVertexCount).setType(ox, oy, oz, type);
              semiTransparentVertexCount++;
            } else {
              solidVertices->at(solidVertexCount) = vertex;
              solidVertices->at(solidVertexCount).offset(x, y, z);
              solidVertices->at(solidVertexCount).setType(ox, oy, oz, type);
              solidVertexCount++;
            }
          }
        }
      }
    }
  }

  int32_t vertexCount = solidVertexCount + semiTransparentVertexCount;

  if (!mesh) {
    mesh = std::make_shared<VertexArray>();
    mesh->addVertexAttributes(BlockVertex::vertexAttributes(), sizeof(BlockVertex));
  }

  Ref<VertexBuffer> buffer = mesh->getVertexBuffer();
  if (buffer->getSize() < vertexCount) {
    int32_t dataSize = glm::min(vertexCount + 1000, MaxVertexCount);
    buffer->bufferDynamicData(*solidVertices, dataSize, 0);
  } else {
    buffer->bufferDynamicSubData(*solidVertices, solidVertexCount, 0, 0);
  }

  buffer->bufferDynamicSubData(*semiTransparentVertices, semiTransparentVertexCount, 0, solidVertexCount);
}

glm::ivec3 Chunk::toChunkCoordinates(const glm::ivec3& globalPosition) {
  return {Math::positiveMod(globalPosition.x, HorizontalSize), globalPosition.y,
          Math::positiveMod(globalPosition.z, HorizontalSize)};
}
