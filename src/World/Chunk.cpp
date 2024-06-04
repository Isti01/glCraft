#include "Chunk.h"

#include "../AssetManager/AssetManager.h"
#include "World.h"

Chunk::Chunk(const glm::ivec2& worldPosition) : worldPosition(worldPosition), aabb(glm::vec3(0), glm::vec3(0)) {
  init();
}

void Chunk::init() {
  solidVertexCount = 0;
  semiTransparentVertexCount = 0;
  mesh = nullptr;
  renderState = RenderState::initial;

  glm::vec3 position = glm::vec3(worldPosition.x, 0, worldPosition.y);
  glm::vec3 maxOffset = glm::vec3(HorizontalSize, VerticalSize, HorizontalSize);
  aabb = AABB{position, position + maxOffset};
}

void Chunk::renderOpaque(const glm::mat4& transform, const Frustum& frustum) {
  TRACE_FUNCTION();
  if (!mesh || !isVisible(frustum)) {
    return;
  }

  shader->bind();
  shader->setMat4("MVP", transform * glm::translate(glm::vec3(worldPosition.x, 0, worldPosition.y)));

  if (solidVertexCount != 0) {
    mesh->renderVertexSubStream(solidVertexCount, 0);
  }
}

void Chunk::renderSemiTransparent(const glm::mat4& transform, const Frustum& frustum) {
  TRACE_FUNCTION();
  if (!mesh || !isVisible(frustum)) {
    return;
  }

  shader->bind();
  shader->setMat4("MVP", transform * glm::translate(glm::vec3(worldPosition.x, 0, worldPosition.y)));

  glDisable(GL_CULL_FACE);
  if (semiTransparentVertexCount != 0) {
    mesh->renderVertexSubStream(semiTransparentVertexCount, solidVertexCount);
  }
  glEnable(GL_CULL_FACE);
}

const BlockData* Chunk::getBlockAtOptimized(const glm::ivec3& pos, const World& world) const {
  const glm::ivec2& worldPos = worldPosition;
  if (pos.y >= 0 && pos.y < Chunk::VerticalSize) {
    if (pos.x >= 0 && pos.x < Chunk::HorizontalSize && pos.z >= 0 && pos.z < Chunk::HorizontalSize) {
      return &data[pos.x][pos.y][pos.z];
    } else {
      return world.getBlockAtIfLoaded(glm::ivec3(pos.x + worldPos.x, pos.y, pos.z + worldPos.y));
    }
  }

  return nullptr;
}

bool hasNonAirAt(const glm::ivec3& pos, const Chunk& chunk, const World& world) {
  const BlockData* block = chunk.getBlockAtOptimized(pos, world);
  return block != nullptr && block->blockClass != BlockData::BlockClass::air;
}

uint8_t calculateOcclusionLevel(const glm::ivec3& blockPos,
                                const glm::ivec3& vertOffset,
                                const Chunk& chunk,
                                const World& world) {
  glm::ivec3 direction = glm::sign(glm::vec3(vertOffset) - glm::vec3(.5));

  uint8_t side1 = hasNonAirAt(blockPos + direction * glm::ivec3(1, 1, 0), chunk, world) ? 1 : 0;
  uint8_t side2 = hasNonAirAt(blockPos + direction * glm::ivec3(0, 1, 1), chunk, world) ? 1 : 0;
  if (side1 && side2) {
    return 0;
  }

  uint8_t corner = hasNonAirAt(blockPos + direction * glm::ivec3(1, 1, 1), chunk, world) ? 1 : 0;
  return 3 - (side1 + side2 + corner);
}

void Chunk::rebuildMesh(const World& world) {
  TRACE_FUNCTION();
  static Ref<std::vector<BlockVertex>> solidVertices = std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);
  static Ref<std::vector<BlockVertex>> semiTransparentVertices =
     std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);

  solidVertexCount = 0;
  semiTransparentVertexCount = 0;

  const std::array<glm::ivec3, 6> offsetsToCheck = {{
     {1, 0, 0},
     {-1, 0, 0},
     {0, 1, 0},
     {0, -1, 0},
     {0, 0, 1},
     {0, 0, -1},
  }};
  {
    TRACE_SCOPE("Chunk::rebuildMesh::WalkBlocks");
    for (int32_t x = HorizontalSize - 1; x >= 0; --x) {
      for (int32_t y = VerticalSize - 1; y >= 0; --y) {
        for (int32_t z = HorizontalSize - 1; z >= 0; --z) {
          glm::ivec3 blockPos = {x, y, z};
          const auto& [type, blockClass] = data[x][y][z];
          if (blockClass == BlockData::BlockClass::air) {
            continue;
          }

          for (const glm::ivec3& offset: offsetsToCheck) {
            const BlockData* block = getBlockAtOptimized(blockPos + offset, world);
            if (block == nullptr) {
              continue;
            }

            bool isSameClass = block->blockClass == blockClass;
            bool isTransparentNextToOpaque =
               block->blockClass == BlockData::BlockClass::solid && blockClass == BlockData::BlockClass::transparent;
            if (isSameClass || isTransparentNextToOpaque) {
              continue;
            }

            for (const auto& vertex: BlockMesh::getVerticesFromDirection(offset)) {
              BlockVertex vert = vertex;
              vert.offset(x, y, z);
              vert.setType(offset, type);

              uint8_t occlusionLevel = 3;
              if (useAmbientOcclusion) {
                if (offset.y == -1) {
                  occlusionLevel = 0;
                } else {
                  occlusionLevel = calculateOcclusionLevel(blockPos, vert.getPosition() - blockPos, *this, world);
                }
              }
              vert.setOcclusionLevel(occlusionLevel);

              if (blockClass == BlockData::BlockClass::semiTransparent ||
                  blockClass == BlockData::BlockClass::transparent) {
                semiTransparentVertices->at(semiTransparentVertexCount) = vert;
                semiTransparentVertexCount++;
              } else {
                solidVertices->at(solidVertexCount) = vert;
                solidVertexCount++;
              }
            }
          }
        }
      }
    }
  }
  {
    TRACE_SCOPE("Chunk::rebuildMesh::WalkBlocks");
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
    renderState = RenderState::ready;
  }
}

glm::ivec3 Chunk::toChunkCoordinates(const glm::ivec3& globalPosition) {
  return {Util::positiveMod(globalPosition.x, HorizontalSize), globalPosition.y,
          Util::positiveMod(globalPosition.z, HorizontalSize)};
}
