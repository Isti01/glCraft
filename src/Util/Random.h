#pragma once

#include "../glCraft.h"

class Random {
  std::mt19937 randomEngine;
  std::uniform_int_distribution<std::mt19937::result_type> distribution;

public:
  Random();

  float getFloat();
  glm::vec2 getVec2();
  glm::vec3 getVec3();
};
