#pragma once

#include "../World/BlockData.h"
#include "../glCraft.h"
#include "VertexArray.h"

class BlockVertex {
  uint8_t yPosition = 0;
  uint8_t xzPosition = 0;
  uint8_t uvCoords = 0;
  uint8_t flags = 0;

  void offsetUv(uint8_t x, uint8_t y) { uvCoords += x | (y << 4); };

public:
  BlockVertex() = default;
  BlockVertex(const glm::ivec3& position, const glm::ivec2& uv);

  /**
   * This is maybe one of the worst hacks in my life ever, but this is how the offset method works:
   * All the vertex coordinates are between 0 and 256, the problem is that the number 256 does not fit
   * into a 8 bit integer. The easiest solution was to set flags when an overflow happens
   */
  void offset(uint32_t x, uint32_t y, uint32_t z) {
    if (yPosition + y > 0xffu) {
      setFlag(0b0010u);
      y = 0;
    }
    if ((xzPosition & 0x0fu) + x > 0xfu) {
      setFlag(0b0100u);
      x = 0;
    }
    if (((xzPosition & 0xf0u) >> 4) + z > 0xfu) {
      setFlag(0b1000u);
      z = 0;
    }
    yPosition += y;
    xzPosition += x | (z << 4);
  }

  void setFlag(uint8_t flag) { flags |= flag; }
  void setAnimated() { setFlag(1); }
  void setType(int32_t offsetX, int32_t offsetY, int32_t offsetZ, BlockData::BlockType type);

  static std::vector<VertexAttribute> vertexAttributes() { return {VertexAttribute(1, VertexAttribute::UInt, 0)}; }
};

static_assert(sizeof(BlockVertex) == sizeof(uint32_t), "The BlockVertex struct must not have padding");
