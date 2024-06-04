#pragma once

#include <Frustum.h>

#include "../Math/AABB.h"
#include "../Rendering/BlockMesh.h"
#include "../Rendering/BlockVertex.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/VertexArray.h"
#include "../glCraft.h"
#include "BlockData.h"

class Persistence;
class World;

class Chunk {
public:
  static constexpr int32_t HorizontalSize = 16;
  static constexpr int32_t VerticalSize = 256;

  static constexpr int32_t BlockCount = HorizontalSize * HorizontalSize * VerticalSize;
  static constexpr int32_t MaxVertexCount = BlockCount * 8;

private:
  enum class RenderState {
    initial,
    ready,
    dirty
  };
  int32_t solidVertexCount;
  int32_t semiTransparentVertexCount;
  Ref<VertexArray> mesh;
  Ref<const ShaderProgram> shader;

  bool useAmbientOcclusion = true;

  RenderState renderState;
  glm::ivec2 worldPosition;

  BlockData data[HorizontalSize][VerticalSize][HorizontalSize];
  AABB aabb;

  void init();

public:
  explicit Chunk(const glm::ivec2& worldPosition);

  void renderOpaque(const glm::mat4& transform, const Frustum& frustum);
  void renderSemiTransparent(const glm::mat4& transform, const Frustum& frustum);
  void rebuildMesh(const World& world);

  [[nodiscard]] bool needsMeshRebuild() const { return !mesh || renderState != RenderState::ready; };
  void setShader(const Ref<const ShaderProgram>& newShader) { shader = newShader; };
  void setDirty() { renderState = RenderState::dirty; };
  void setUseAmbientOcclusion(bool enabled) {
    if (enabled == useAmbientOcclusion) {
      return;
    }

    setDirty();
    useAmbientOcclusion = enabled;
  };

  [[nodiscard]] bool isVisible(const Frustum& frustum) const {
    return frustum.IsBoxVisible(aabb.minPoint, aabb.maxPoint);
  };

  void placeBlock(BlockData block, const glm::ivec3& position) {
    placeBlock(block, position.x, position.y, position.z);
  }

  void placeBlock(BlockData block, int32_t x, int32_t y, int32_t z) {
    assert(isInBounds(x, y, z));

    renderState = RenderState::dirty;
    data[x][y][z] = block;
  }

  [[nodiscard]] float distanceToPoint(const glm::vec2& point) const {
    glm::vec2 referencePoint = {glm::clamp(point.x, (float) worldPosition.x, (float) worldPosition.x + 16.0f),
                                glm::clamp(point.y, (float) worldPosition.y, (float) worldPosition.y + 16.0f)};

    return glm::distance(referencePoint, point);
  }

  [[nodiscard]] const BlockData* getBlockAt(const glm::ivec3& position) const {
    return &data[position.x][position.y][position.z];
  }

  static bool isInBounds(int32_t x, int32_t y, int32_t z) {
    return x >= 0 && x < HorizontalSize && y >= 0 && y < VerticalSize && z >= 0 && z < HorizontalSize;
  }

  [[nodiscard]] const BlockData* getBlockAtOptimized(const glm::ivec3& pos, const World& world) const;
  static bool isValidPosition(glm::ivec3 position) { return position.y >= 0 && position.y < VerticalSize; }
  static glm::ivec3 toChunkCoordinates(const glm::ivec3& globalPosition);

  glm::ivec2 getPosition() { return worldPosition; }
  friend Persistence;
};
