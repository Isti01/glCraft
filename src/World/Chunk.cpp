#include "Chunk.h"

#include "../AssetManager/AssetManager.h"
#include "World.h"

Chunk::Chunk(const glm::ivec2& worldPosition) : worldPosition(worldPosition) {
  init();
}

void Chunk::init() {
  solidVertexCount = 0;
  semiTransparentVertexCount = 0;
  mesh = nullptr;
  renderState = RenderState::initial;
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

uint8_t calculateOcclusionLevel(const glm::ivec3& blockPos,
                                const glm::ivec3& vertOffset,
                                const Chunk& chunk,
                                const World& world) {
  glm::ivec3 direction = glm::sign(glm::vec3(vertOffset) - glm::vec3(.5));

  const BlockData* side1Block = chunk.getBlockAtOptimized(blockPos + direction * glm::ivec3(1, 1, 0), world);
  const BlockData* side2Block = chunk.getBlockAtOptimized(blockPos + direction * glm::ivec3(0, 1, 1), world);
  const BlockData* cornerBlock = chunk.getBlockAtOptimized(blockPos + direction * glm::ivec3(1, 1, 1), world);

  uint8_t side1 = (side1Block != nullptr && side1Block->blockClass != BlockData::BlockClass::air) ? 1 : 0;
  uint8_t side2 = (side2Block != nullptr && side2Block->blockClass != BlockData::BlockClass::air) ? 1 : 0;
  uint8_t corner = (cornerBlock != nullptr && cornerBlock->blockClass != BlockData::BlockClass::air) ? 1 : 0;

  if (side1 && side2) {
    return 0;
  }

  return 3 - (side1 + side2 + corner);
}

void Chunk::createMesh(const World& world) {
  static Ref<std::vector<BlockVertex>> solidVertices = std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);
  static Ref<std::vector<BlockVertex>> semiTransparentVertices =
     std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);

  solidVertexCount = 0;
  semiTransparentVertexCount = 0;

  // used tuple, because glm::ivec3 cannot be destructured
  const std::array<glm::ivec3, 6> offsetsToCheck = {{
     {1, 0, 0},
     {-1, 0, 0},
     {0, 1, 0},
     {0, -1, 0},
     {0, 0, 1},
     {0, 0, -1},
  }};

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
          if (block != nullptr && block->blockClass == blockClass) {
            continue;
          }

          for (const auto& vertex: BlockMesh::getVerticesFromDirection(offset)) {
            BlockVertex vert = vertex;
            vert.offset(x, y, z);
            vert.setType(offset, type);
            if (useAmbientOcclusion) {
              if (offset.y == -1) {
                vert.setOcclusionLevel(0);
              } else {
                vert.setOcclusionLevel(calculateOcclusionLevel(blockPos, vert.getPosition() - blockPos, *this, world));
              }
            } else {
              vert.setOcclusionLevel(3);
            }

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
  return {Util::positiveMod(globalPosition.x, HorizontalSize), globalPosition.y,
          Util::positiveMod(globalPosition.z, HorizontalSize)};
}
