#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(int seed) : seed(seed), noise(seed) {
  noise.SetFractalOctaves(7);
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}
