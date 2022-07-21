#pragma once

#include "../World/BlockData.h"
#include "../glCraft.h"
#include "VertexArray.h"

/**
 * Data layout:
 *  00-08: y  coordinates
 *  09-13: x  coordinates
 *  14-18: z  coordinates
 *  19-20: uv coordinates
 *  20-27: texture index
 *  28-28: animation flag
 *  29-30: occlusion
 *  31-31: reserved
 */
class BlockVertex {
private:
  uint32_t data = 0;

  void setUv(bool x, bool y);
  void setTexture(uint8_t x, uint8_t y);

public:
  BlockVertex() = default;
  BlockVertex(const glm::ivec3& position, const glm::bvec2& uv);

  void offset(uint32_t x, uint32_t y, uint32_t z);
  void setAnimated();
  void setType(const glm::ivec3& offset, BlockData::BlockType type);
  [[nodiscard]] glm::ivec3 getPosition() const;

  static std::vector<VertexAttribute> vertexAttributes() { return {VertexAttribute(1, VertexAttribute::UInt, 0)}; }
  void setOcclusionLevel(uint8_t occlusionLevel);
};

static_assert(sizeof(BlockVertex) == sizeof(uint32_t), "The BlockVertex struct must not have padding");
