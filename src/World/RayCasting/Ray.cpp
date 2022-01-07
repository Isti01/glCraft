#include "Ray.h"

#include "AxisPlane.h"

Ray::Ray(glm::vec3 position, glm::vec3 direction, World& world, float reach = 10.0f)
    : position(position),
      direction(direction),
      world(world) {
  successful = false;

  std::array<AxisPlane, 3> planes = {{
     AxisPlane{{1, 0, 0}, position, direction},
     AxisPlane{{0, 1, 0}, position, direction},
     AxisPlane{{0, 0, 1}, position, direction},
  }};
  std::sort(planes.begin(), planes.end());

  while (!successful && planes[0].getHitDistance() <= reach) {
    glm::ivec3 blockPosition = planes[0].getBlockPosition();
    BlockData block = world.getBlockAt(blockPosition);
    if (block.type != BlockData::BlockType::air) {
      successful = true;
      hitTarget = {blockPosition, block};
    } else {
      planes[0].advanceOffset();
      std::sort(planes.begin(), planes.end());
    }
  }
}
