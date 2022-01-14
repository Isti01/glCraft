#pragma once

#include "../AssetManager/AssetManager.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/Texture.h"
#include "../glCraft.h"
#include "Chunk.h"
#include "WorldGenerator.h"

class HashVec2 {
public:
  size_t operator()(const glm::ivec2& coord) const noexcept {
    return std::hash<int>{}(coord.x) | (std::hash<int>{}(coord.y) << sizeof(int) * 8);
  };
};

class World {
  std::unordered_map<glm::ivec2, Ref<Chunk>, HashVec2> chunks;
  Ref<const Texture> textureAtlas;
  Ref<const ShaderProgram> defaultShader;
  WorldGenerator generator;

  Ref<Chunk> generateOrLoadChunk(glm::ivec2 position) {
    Ref<Chunk> chunk = std::make_shared<Chunk>(position);
    generator.populateChunk(chunk);

    return chunk;  // todo make the chunks persistent
  }

public:
  World(int seed = 1337);

  Ref<Chunk> getChunk(glm::ivec2 position);
  void addChunk(glm::ivec2 position, const Ref<Chunk>& chunk) { chunks[position] = chunk; };
  [[nodiscard]] static glm::ivec2 getChunkIndex(glm::ivec3 position);

  [[nodiscard]] BlockData getBlockAt(glm::ivec3 position);
  bool placeBlock(BlockData block, glm::ivec3 position);

  void render(glm::vec3 playerPos, glm::mat4 transform);

  static bool isValidBlockPosition(glm::ivec3 position);
};
