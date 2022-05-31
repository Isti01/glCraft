#pragma once

#include "../World/BlockData.h"
#include "../glCraft.h"
#include "VertexArray.h"

/**
 * Data layout:
 *  00-09: y  coordinates
 *  09-14: x  coordinates
 *  14-19: z  coordinates
 *  19-27: uv coordinates
 *  27-28: animation flag
 *  28-29: reserved
 *  29-32: normals
 */
class BlockVertex {
private:
  uint32_t data = 0;

  void offsetUv(uint8_t x, uint8_t y);
public:
  BlockVertex() = default;
  BlockVertex(const glm::ivec3& position, const glm::ivec2& uv, uint8_t normalIndex);

  void offset(uint32_t x, uint32_t y, uint32_t z);
  void setAnimated();
  void setNormal(uint8_t normalIndex);
  void setType(int32_t offsetX, int32_t offsetY, int32_t offsetZ, BlockData::BlockType type);

  static std::vector<VertexAttribute> vertexAttributes() { return {VertexAttribute(1, VertexAttribute::UInt, 0)}; }
};

static_assert(sizeof(BlockVertex) == sizeof(uint32_t), "The BlockVertex struct must not have padding");
