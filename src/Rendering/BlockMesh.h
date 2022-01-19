#pragma once

#include "BlockVertex.h"

struct BlockMesh {
  static const std::array<BlockVertex, 6>& getVerticesFromDirection(int32_t x, int32_t y, int32_t z) {
    assert(glm::abs(x) + glm::abs(y) + glm::abs(z) == 1);

    switch (x) {
      case 1:
        return vertices[1];
      case -1:
        return vertices[2];
      default:
        break;
    }

    switch (y) {
      case 1:
        return vertices[0];
      case -1:
        return vertices[5];
      default:
        break;
    }

    switch (z) {
      case 1:
        return vertices[4];
      case -1:
        return vertices[3];
      default:
        break;
    }

    assert(false);
    return vertices[0];
  }

  static const std::array<std::array<BlockVertex, 6>, 6> vertices;
};
