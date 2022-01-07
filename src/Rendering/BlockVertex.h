#pragma once

#include "../World/BlockData.h"
#include "../glCraft.h"
#include "VertexArray.h"

struct BlockVertex {
  glm::vec3 position;
  glm::vec2 uvCoords;
  //  uint8_t yPosition;
  //  uint8_t xzPosition;
  //  uint8_t uvCoordsAndTexture; // todo one vertex should take up less than 32 bits of space

  void offset(int32_t x, int32_t y, int32_t z) { position += glm::vec3(x, y, z); }

  void setType(int32_t offsetX, int32_t offsetY, int32_t offsetZ, BlockData::BlockType type) {
    switch (type) {
      case BlockData::BlockType::grass:
        if (offsetY == 1) {
          uvCoords += glm::vec2(0, 0);
        } else if (offsetY == -1) {
          uvCoords += glm::vec2(2, 0);
        } else {
          uvCoords += glm::vec2(3, 0);
        }
        break;
      case BlockData::BlockType::dirt:
        uvCoords += glm::vec2(2, 0);
        break;
      case BlockData::BlockType::sand:
        uvCoords += glm::vec2(2, 1);
        break;
      case BlockData::BlockType::stone:
        uvCoords += glm::vec2(1, 0);
        break;
      case BlockData::BlockType::cobble_stone:
        uvCoords += glm::vec2(0, 1);
        break;
      case BlockData::BlockType::glass:
        uvCoords += glm::vec2(1, 3);
        break;
      case BlockData::BlockType::oak_wood:
        if (offsetY == 1 || offsetY == -1) {
          uvCoords += glm::vec2(5, 1);
        } else {
          uvCoords += glm::vec2(4, 1);
        }
        break;
      case BlockData::BlockType::oak_leaves:
        uvCoords += glm::vec2(4, 3);
        break;
      case BlockData::BlockType::air:
        assert(false);
        break;
      default:
        assert(false);
    }
  }

  static std::vector<VertexAttribute> vertexAttributes() {
    return {VertexAttribute(3, VertexAttribute::Float, 0),
            VertexAttribute(2, VertexAttribute::Float, sizeof(glm::vec3))};
  }
};

//static_assert(sizeof(CubeVertex) == sizeof(uint8_t) * 3, "The CubeVertex struct must not have padding");
