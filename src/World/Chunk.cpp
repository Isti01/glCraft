#include "Chunk.h"

#include "../AssetManager/AssetManager.h"
#include "../Util/Math.h"

Chunk::Chunk(const glm::ivec2& worldPosition) : worldPosition(worldPosition) {}

void Chunk::render(const glm::mat4& transform) {
  if (!mesh || renderState != RenderState::ready) {
    mesh = createMesh();
    renderState = RenderState::ready;
  }

  const auto& shader = AssetManager::instance().loadShaderProgram("assets/shaders/default");

  shader->bind();
  shader->setMat4("MVP", transform * glm::translate(glm::vec3(worldPosition.x, 0, worldPosition.y)));

  mesh->renderVertexSubStream(vertexCount);
}

Ref<VertexArray> Chunk::createMesh() {
  static Ref<std::vector<BlockVertex>> vertices = std::make_shared<std::vector<BlockVertex>>(VertexCount);

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

  for (int x = 0; x < HorizontalSize; x++) {
    for (int y = 0; y < VerticalSize; y++) {
      for (int z = 0; z < HorizontalSize; z++) {
        const BlockData::BlockType type = data[x][y][z].type;
        const bool transparent = BlockData::isTransparent(type);
        if (type == BlockData::BlockType::air) continue;

        for (const auto& [ox, oy, oz]: offsetsToCheck) {
          // todo fix this
          if (isInBounds(x + ox, y + oy, z + oz) && data[x + ox][y + oy][z + oz].type != BlockData::BlockType::air &&
              transparent == BlockData::isTransparent(data[x + ox][y + oy][z + oz].type)) {
            continue;
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

  if (mesh) {
    mesh->getVertexBuffer()->bufferDynamicVertexSubData(*vertices);
    return mesh;
  } else {
    return std::make_shared<VertexArray>(*vertices, BlockVertex::vertexAttributes());
  }
}

bool Chunk::isInBounds(int32_t x, int32_t y, int32_t z) {
  return x >= 0 && x < HorizontalSize && y >= 0 && y < VerticalSize && z >= 0 && z < HorizontalSize;
}

void Chunk::placeBlock(BlockData block, const glm::ivec3& position) {
  if (!isInBounds(position.x, position.y, position.z)) {
    std::stringstream ss("Chunk out of bounds, cannot place block at: ");
    ss << position.x << " " << position.y << " " << position.z;
    throw std::exception(ss.str().c_str());
  }

  renderState = RenderState::dirty;
  data[position.x][position.y][position.z] = block;
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
