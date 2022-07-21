#include "BlockVertex.h"

BlockVertex::BlockVertex(const glm::ivec3& position, const glm::bvec2& uv) {
  offset(position.x, position.y, position.z);
  setUv(uv.x, uv.y);
}

void BlockVertex::setUv(bool x, bool y) {
  uint8_t uv = x | (y << 1);
  assert((((data >> 19) & 0xff) + uv) <= 0xff && "UV Coordinates are out of bounds");

  data += uv << 19;
};

void BlockVertex::setTexture(uint8_t x, uint8_t y) {
  assert(x < 16 && "Coordinate is out of bounds");
  assert(y < 16 && "Coordinate is out of bounds");

  uint16_t uv = x | (y << 4);
  assert(uv <= 0xff && "Texture index is out of bounds");

  data += uv << 20;
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
  data |= 0b1 << 28;
}

glm::ivec3 BlockVertex::getPosition() const {
  return {(data >> 9) & 0x1fu, data & 0x1ffu, (data >> 14) & 0x1fu};
}

void BlockVertex::setOcclusionLevel(uint8_t occlusionLevel) {
  assert(occlusionLevel < 4 && "The occlusion level is out of bounds");
  data |= occlusionLevel << 29;
}

void BlockVertex::setType(const glm::ivec3& offset, BlockData::BlockType type) {
  switch (type) {
    case BlockData::BlockType::bedrock:
      setTexture(1, 1);
      break;
    case BlockData::BlockType::planks:
      setTexture(4, 0);
      break;
    case BlockData::BlockType::water:
      setAnimated();
      setTexture(13, 12);
      break;
    case BlockData::BlockType::lava:
      setAnimated();
      setTexture(13, 14);
      break;
    case BlockData::BlockType::iron:
      setTexture(6, 1);
      break;
    case BlockData::BlockType::diamond:
      setTexture(8, 1);
      break;
    case BlockData::BlockType::gold:
      setTexture(7, 1);
      break;
    case BlockData::BlockType::obsidian:
      setTexture(5, 2);
      break;
    case BlockData::BlockType::sponge:
      setTexture(0, 3);
      break;
    case BlockData::BlockType::grass:
      if (offset.y == 1) {
        setTexture(0, 0);
      } else if (offset.y == -1) {
        setTexture(2, 0);
      } else {
        setTexture(3, 0);
      }
      break;
    case BlockData::BlockType::dirt:
      setTexture(2, 0);
      break;
    case BlockData::BlockType::sand:
      setTexture(2, 1);
      break;
    case BlockData::BlockType::stone:
      setTexture(1, 0);
      break;
    case BlockData::BlockType::cobblestone:
      setTexture(0, 1);
      break;
    case BlockData::BlockType::glass:
      setTexture(1, 3);
      break;
    case BlockData::BlockType::oak_wood:
      if (offset.y == 1 || offset.y == -1) {
        setTexture(5, 1);
      } else {
        setTexture(4, 1);
      }
      break;
    case BlockData::BlockType::oak_leaves:
      setTexture(4, 3);
      break;
    case BlockData::BlockType::air:
      assert(false);
      break;
    default:
      assert(false);
  }
}
