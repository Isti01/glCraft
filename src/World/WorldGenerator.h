#pragma once

#include "../glCraft.h"
#include "Chunk.h"

class WorldGenerator {
  int32_t seed;
  FastNoiseLite noise;

public:
  WorldGenerator(int32_t seed);
  [[nodiscard]] int32_t getSeed() const { return seed; };

  void populateChunk(const Ref<Chunk>& chunkRef) {
    Chunk& chunk = *chunkRef;

    glm::ivec2 worldPosition = chunk.getPosition();
    glm::vec2 position = worldPosition;


    for (int32_t x = 0; x < Chunk::HorizontalSize; x++) {
      for (int32_t z = 0; z < Chunk::HorizontalSize; z++) {
        float noiseX = (position.x + static_cast<float>(x));
        float noiseY = (position.y + static_cast<float>(z));
        float noiseValue = noise.GetNoise(noiseX, noiseY) / 2.0f + 1.0f;
        int32_t height = 40 + static_cast<int>(noiseValue * 40);

        int32_t worldX = worldPosition.x + x;
        int32_t worldZ = worldPosition.y + z;

        for (int32_t y = 0; y < height; y++) {
          int32_t dy = height - y;
          BlockData::BlockType blockToPlace = BlockData::BlockType::stone;

          if (dy == 1) {
            blockToPlace = BlockData::BlockType::grass;
          } else if (dy < 5) {
            blockToPlace = BlockData::BlockType::dirt;
          }

          chunk.placeBlock(blockToPlace, glm::ivec3(x, y, z));
        }

        for (int32_t y = 64; y > height; y--) {
          // todo add water
          chunk.placeBlock(BlockData::BlockType::glass, glm::ivec3(x, y, z));
        }

        // todo add bedrock
        chunk.placeBlock(BlockData::BlockType::cobble_stone, glm::ivec3(x, 0, z));
      }
    }
  }
};