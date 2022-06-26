#include "AxisPlane.h"
#include "WorldRayCast.h"

WorldRayCast::WorldRayCast(glm::vec3 position, glm::vec3 direction, World& world, float reach = 10.0f) {
  successful = false;

  std::array<AxisPlane, 3> planes = {{
     AxisPlane{{1, 0, 0}, position, direction},
     AxisPlane{{0, 1, 0}, position, direction},
     AxisPlane{{0, 0, 1}, position, direction},
  }};

  std::sort(planes.begin(), planes.end());
  glm::vec3 prevClosestHits[2] = {position, position};  // the player might be inside a block
  bool hasNeighbor = false;

  while (!successful && planes[0].getHitDistance() <= reach) {
    std::optional<glm::ivec3> maybeBlockPosition =
       AxisPlane::rayHitsToBlockPosition(planes[0].getHitPosition(), prevClosestHits[1]);

    if (maybeBlockPosition.has_value() && World::isValidBlockPosition(maybeBlockPosition.value())) {
      glm::vec3 blockPosition = maybeBlockPosition.value();
      const BlockData* block = world.getBlockAt(blockPosition);

      if (block->type != BlockData::BlockType::air) {
        successful = true;
        std::optional<glm::ivec3> maybeNeighbor =
           AxisPlane::rayHitsToBlockPosition(prevClosestHits[0], prevClosestHits[1]);

        hitTarget = {blockPosition, *block, maybeNeighbor.value(), hasNeighbor && maybeNeighbor.has_value()};
      }
    }

    hasNeighbor = true;
    prevClosestHits[0] = prevClosestHits[1];
    prevClosestHits[1] = planes[0].getHitPosition();
    planes[0].advanceOffset();
    std::sort(planes.begin(), planes.end());
  }
}
