#include "BlockVertex.h"

BlockVertex::BlockVertex(const glm::ivec3& position, const glm::ivec2& uv, bool animated)
    : flags(animated ? 0b1u : 0b0u) {
  offset(position.x, position.y, position.z);
  offsetUv(uv.x, uv.y);
}


void BlockVertex::setType(int32_t offsetX, int32_t offsetY, int32_t offsetZ, BlockData::BlockType type) {
  switch (type) {
    case BlockData::BlockType::grass:
      if (offsetY == 1) {
        offsetUv(0, 0);
      } else if (offsetY == -1) {
        offsetUv(2, 0);
      } else {
        offsetUv(3, 0);
      }
      break;
    case BlockData::BlockType::dirt:
      offsetUv(2, 0);
      break;
    case BlockData::BlockType::sand:
      offsetUv(2, 1);
      break;
    case BlockData::BlockType::stone:
      offsetUv(1, 0);
      break;
    case BlockData::BlockType::cobblestone:
      offsetUv(0, 1);
      break;
    case BlockData::BlockType::glass:
      offsetUv(1, 3);
      break;
    case BlockData::BlockType::oak_wood:
      if (offsetY == 1 || offsetY == -1) {
        offsetUv(5, 1);
      } else {
        offsetUv(4, 1);
      }
      break;
    case BlockData::BlockType::oak_leaves:
      offsetUv(4, 3);
      break;
    case BlockData::BlockType::air:
      assert(false);
      break;
    default:
      assert(false);
  }
}
