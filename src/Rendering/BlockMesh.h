#pragma once

#include "BlockVertex.h"

struct BlockMesh {
  enum class Faces {
    top = 0,
    east = 1,
    west = 2,
    north = 3,
    south = 4,
    bottom = 5  // the enum values match the indices in the vertices array
  };

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

  constexpr static const std::array<std::array<BlockVertex, 6>, 6> vertices = {{
     {{
        // top
        {{-1, 1, 1}, {1, 0}},
        {{1, 1, 1}, {1, 1}},
        {{-1, 1, -1}, {0, 0}},
        {{1, 1, 1}, {1, 1}},
        {{1, 1, -1}, {0, 1}},
        {{-1, 1, -1}, {0, 0}},
     }},
     {{
        // +x east
        {{1, 1, 1}, {1, 0}},
        {{1, -1, 1}, {1, 1}},
        {{1, 1, -1}, {0, 0}},
        {{1, -1, 1}, {1, 1}},
        {{1, -1, -1}, {0, 1}},
        {{1, 1, -1}, {0, 0}},
     }},
     {{
        //-x west
        {{-1, 1, -1}, {1, 0}},
        {{-1, -1, -1}, {1, 1}},
        {{-1, 1, 1}, {0, 0}},
        {{-1, -1, -1}, {1, 1}},
        {{-1, -1, 1}, {0, 1}},
        {{-1, 1, 1}, {0, 0}},
     }},
     {{
        //-z north
        {{1, 1, -1}, {1, 0}},
        {{1, -1, -1}, {1, 1}},
        {{-1, 1, -1}, {0, 0}},
        {{1, -1, -1}, {1, 1}},
        {{-1, -1, -1}, {0, 1}},
        {{-1, 1, -1}, {0, 0}},
     }},

     {{
        // +z south
        {{-1, 1, 1}, {1, 0}},
        {{-1, -1, 1}, {1, 1}},
        {{1, 1, 1}, {0, 0}},
        {{-1, -1, 1}, {1, 1}},
        {{1, -1, 1}, {0, 1}},
        {{1, 1, 1}, {0, 0}},
     }},
     {{
        // bottom
        {{1, -1, 1}, {1, 0}},
        {{-1, -1, 1}, {1, 1}},
        {{1, -1, -1}, {0, 0}},
        {{-1, -1, 1}, {1, 1}},
        {{-1, -1, -1}, {0, 1}},
        {{1, -1, -1}, {0, 0}},
     }},
  }};
};
