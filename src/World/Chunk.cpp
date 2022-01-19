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

  mesh->renderVertexSubStream(vertexCount);
}

void Chunk::createMesh(const World& world) {
  static Ref<std::vector<BlockVertex>> vertices = std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);

  vertexCount = 0;

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
            vertices->at(vertexCount) = vertex;
            vertices->at(vertexCount).offset(x, y, z);
            vertices->at(vertexCount).setType(ox, oy, oz, type);
            vertexCount++;
          }
        }
      }
    }
  }

  if (mesh && mesh->getVertexBuffer()->getSize() >= vertexCount) {
    mesh->getVertexBuffer()->bufferDynamicSubData(*vertices, vertexCount);
  } else {
    mesh = std::make_shared<VertexArray>();
    mesh->addVertexAttributes(BlockVertex::vertexAttributes(), sizeof(BlockVertex));
    int32_t dataSize = glm::min(vertexCount + 1000, MaxVertexCount);

    mesh->getVertexBuffer()->bufferDynamicData(*vertices, dataSize);
  }
}

bool Chunk::isInBounds(int32_t x, int32_t y, int32_t z) {
  return x >= 0 && x < HorizontalSize && y >= 0 && y < VerticalSize && z >= 0 && z < HorizontalSize;
}

void Chunk::placeBlock(BlockData block, const glm::ivec3& position) {
  assert(isInBounds(position.x, position.y, position.z));

  renderState = RenderState::dirty;
  data[position.x][position.y][position.z] = block;
}

void Chunk::placeBlock(BlockData block, int32_t x, int32_t y, int32_t z) {
  assert(isInBounds(x, y, z));

  renderState = RenderState::dirty;
  data[x][y][z] = block;
}

BlockData Chunk::getBlockAt(const glm::ivec3& position) const {
  return data[position.x][position.y][position.z];
}

bool Chunk::isValidPosition(glm::ivec3 position) {
  return position.y >= 0 && position.y < VerticalSize;
}

glm::ivec3 Chunk::toChunkCoordinates(const glm::ivec3& globalPosition) {
  return {Math::positiveMod(globalPosition.x, HorizontalSize), globalPosition.y,
          Math::positiveMod(globalPosition.z, HorizontalSize)};
}

glm::ivec2 Chunk::getPosition() {
  return worldPosition;
}
