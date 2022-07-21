#pragma once

#include "../glCraft.h"

struct Image {
  uint32_t width, height;
  std::vector<uint8_t> data;

  Image subImage(glm::uvec2 offset, glm::uvec2 extent) const;
};