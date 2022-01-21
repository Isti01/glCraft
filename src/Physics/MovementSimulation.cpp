#include "MovementSimulation.h"

#include "../Math/Ray.h"
#include "../Math/AABB.h"

bool MovementSimulation::canMove(const glm::vec3& from, const glm::vec3& to, World& world) {
  AABB playerBox(AABB::PlayerBoxOffsets);

  glm::vec3 movementDirection = from - to;
  for (auto& point: playerBox.points) { point += to; }

  for (const auto& point: playerBox.points) {
    // I used ray casting to determine which blocks should be used in collision calculation
    if (Ray ray{point, movementDirection, world, 3.0f}) {
      glm::vec3 position = ray.getHitTarget().position;

      if (AABB::fromBlockPosition(position).intersect(playerBox)) {
        return false;
      }
    }
  }

  return true;
}
