#include "BlockVertex.h"

BlockVertex::BlockVertex(const glm::ivec3& position, const glm::ivec2& uv, uint8_t normalIndex) {
  offset(position.x, position.y, position.z);
  offsetUv(uv.x, uv.y);
  setNormal(normalIndex);
}

void BlockVertex::offsetUv(uint8_t x, uint8_t y) {
  assert(x < 16 && "Coordinate is out of bounds");
  assert(y < 16 && "Coordinate is out of bounds");

  uint8_t uv = x | (y << 4);
  assert((((data >> 19) & 0xff) + uv) <= 0xff && "UV Coordinates are out of bounds");

  data += uv << 19;
};

void BlockVertex::offset(uint32_t x, uint32_t y, uint32_t z) {
  assert((((data >> 9) & 0x1fu) + x) <= 16 && "Coordinate is out of bounds");
  assert((((data >> 14) & 0x1fu) + z) <= 16 && "Coordinate is out of bounds");
  assert(((data & 0x1ffu) + y) <= 256 && "Coordinate is out of bounds");

  data += y;
  data += x << 9;
  data += z << 14;
}

void BlockVertex::setAnimated() {
  data |= 0b1 << 27;
}

void BlockVertex::setNormal(uint8_t normalIndex) {
  assert(normalIndex < 6);
  data |= normalIndex << 29;
}

void BlockVertex::setType(int32_t offsetX, int32_t offsetY, int32_t offsetZ, BlockData::BlockType type) {
  switch (type) {
    case BlockData::BlockType::bedrock:
      offsetUv(1, 1);
      break;
    case BlockData::BlockType::planks:
      offsetUv(4, 0);
      break;
    case BlockData::BlockType::water:
      setAnimated();
      offsetUv(13, 12);
      break;
    case BlockData::BlockType::lava:
      setAnimated();
      offsetUv(13, 14);
      break;
    case BlockData::BlockType::iron:
      offsetUv(6, 1);
      break;
    case BlockData::BlockType::diamond:
      offsetUv(8, 1);
      break;
    case BlockData::BlockType::gold:
      offsetUv(7, 1);
      break;
    case BlockData::BlockType::obsidian:
      offsetUv(5, 2);
      break;
    case BlockData::BlockType::sponge:
      offsetUv(0, 3);
      break;
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
