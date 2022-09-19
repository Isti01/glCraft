#include "MovementSimulation.h"

#include "../Math/WorldRayCast.h"
#include "../Scene/Player.h"

bool MovementSimulation::canMove(const glm::vec3& from, const glm::vec3& to, World& world) {
  glm::vec3 movementDirection = from - to;

  AABB playerAABBCopy = Player::PlayerAABB;
  playerAABBCopy.minPoint += to;
  playerAABBCopy.maxPoint += to;

  for (auto point: Player::PlayerBoundingBox) {
    point += to;
    // I used ray casting to determine which blocks should be used in collision calculation
    if (WorldRayCast ray{point, movementDirection, world, 3.0f}) {
      glm::vec3 position = ray.getHitTarget().position;

      if (AABB::fromBlockPosition(position).intersect(playerAABBCopy)) {
        return false;
      }
    }
  }

  return true;
}
