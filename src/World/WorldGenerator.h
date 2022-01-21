#pragma once

#include "../glCraft.h"
#include "Chunk.h"

class WorldGenerator {
  int32_t seed;
  FastNoiseLite noise;

public:
  WorldGenerator(int32_t seed);
  [[nodiscard]] int32_t getSeed() const { return seed; };

  void populateChunk(const Ref<Chunk>& chunkRef);
};