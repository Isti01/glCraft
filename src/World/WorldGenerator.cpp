#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(int32_t seed) : seed(seed), noise(seed) {
  noise.SetFractalOctaves(5);
  noise.SetFractalLacunarity(1.75);
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  noise.SetFractalType(FastNoiseLite::FractalType_FBm);
}

void WorldGenerator::populateChunk(const Ref<Chunk>& chunkRef) {
  TRACE_FUNCTION();
  Chunk& chunk = *chunkRef;

  glm::ivec2 worldPosition = chunk.getPosition();
  glm::vec2 position = worldPosition;


  for (int32_t x = 0; x < Chunk::HorizontalSize; x++) {
    for (int32_t z = 0; z < Chunk::HorizontalSize; z++) {
      float noiseX = (position.x + static_cast<float>(x));
      float noiseY = (position.y + static_cast<float>(z));
      float noiseValue = noise.GetNoise(noiseX, noiseY) / 2.0f + 0.5f;
      int32_t height = 45 + static_cast<int32_t>(noiseValue * 45);

      for (int32_t y = 0; y < height; y++) {
        int32_t dy = height - y;
        BlockData::BlockType blockToPlace = BlockData::BlockType::stone;

        if (dy == 1) {
          if (y <= 64 && y >= 63) {
            blockToPlace = BlockData::BlockType::sand;
          } else if (y < 63) {
            blockToPlace = BlockData::BlockType::stone;
          } else {
            blockToPlace = BlockData::BlockType::grass;
          }
        } else if (dy < 5) {
          if (y < 64) {
            blockToPlace = BlockData::BlockType::stone;
          } else {
            blockToPlace = BlockData::BlockType::dirt;
          }
        }

        chunk.placeBlock(blockToPlace, x, y, z);
      }

      for (int32_t y = 64; y >= height; y--) { chunk.placeBlock(BlockData::BlockType::water, x, y, z); }
      chunk.placeBlock(BlockData::BlockType::bedrock, x, 0, z);
    }
  }
}
