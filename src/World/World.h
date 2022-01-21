#pragma once

#include "../AssetManager/AssetManager.h"
#include "../Persistence/Persistence.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/Texture.h"
#include "../glCraft.h"
#include "Chunk.h"
#include "WorldGenerator.h"

class HashVec2 {
public:
  size_t operator()(const glm::ivec2& coord) const noexcept {
    return std::hash<int32_t>{}(coord.x) | (std::hash<int32_t>{}(coord.y) << sizeof(int32_t) * 8);
  };
};

class World {
  std::unordered_map<glm::ivec2, Ref<Chunk>, HashVec2> chunks;
  Ref<const Texture> textureAtlas;
  Ref<const ShaderProgram> shader;
  Ref<Persistence> persistence;
  WorldGenerator generator;

  int32_t viewDistance = 10;
  float textureAnimation = 0;
  static constexpr float TextureAnimationSpeed = 2;

  Ref<Chunk> generateOrLoadChunk(glm::ivec2 position);

public:
  explicit World(const Ref<Persistence>& persistence, int32_t seed = 1337);

  Ref<Chunk> getChunk(glm::ivec2 position);
  void addChunk(glm::ivec2 position, const Ref<Chunk>& chunk) { chunks[position] = chunk; };
  [[nodiscard]] static glm::ivec2 getChunkIndex(glm::ivec3 position);

  [[nodiscard]] BlockData getBlockAt(glm::ivec3 position);
  [[nodiscard]] std::optional<BlockData> getBlockAtIfLoaded(glm::ivec3 position) const;
  [[nodiscard]] bool isChunkLoaded(glm::ivec2 position) const;
  bool placeBlock(BlockData block, glm::ivec3 position);

  void update(const glm::vec3& playerPosition, float deltaTime);
  void render(glm::vec3 playerPos, glm::mat4 transform);

  static bool isValidBlockPosition(glm::ivec3 position);
  void setTextureAtlas(const Ref<const Texture>& texture);
};
