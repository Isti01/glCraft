#pragma once

#include "../Rendering/BlockMesh.h"
#include "../Rendering/BlockVertex.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/VertexArray.h"
#include "../glCraft.h"
#include "BlockData.h"

class World;

class Chunk {
public:
  constexpr static int32_t HorizontalSize = 16;
  constexpr static int32_t VerticalSize = 256;

  constexpr static int32_t BlockCount = HorizontalSize * HorizontalSize * VerticalSize;
  constexpr static int32_t MaxVertexCount = BlockCount * 8;

private:
  enum class RenderState {
    initial,
    ready,
    dirty
  };
  int32_t vertexCount = 0;
  Ref<VertexArray> mesh = nullptr;
  Ref<const ShaderProgram> shader;

  RenderState renderState = RenderState::initial;
  glm::ivec2 worldPosition;

  BlockData data[HorizontalSize][VerticalSize][HorizontalSize];

  [[nodiscard]] static bool isInBounds(int32_t x, int32_t y, int32_t z);

  void createMesh(const World& world);


public:
  explicit Chunk(const glm::ivec2& worldPosition);
  void render(const glm::mat4& transform, const World& world);

  void setDirty() { renderState = RenderState::dirty; };
  void placeBlock(BlockData block, const glm::ivec3& position);
  [[nodiscard]] BlockData getBlockAt(const glm::ivec3& position) const;

  static bool isValidPosition(glm::ivec3 position);
  static glm::ivec3 toChunkCoordinates(const glm::ivec3& globalPosition);
  glm::ivec2 getPosition();
};
