#pragma once

#include "../World/World.h"
#include "../glCraft.h"

struct HitTarget {
  glm::vec3 position;
  BlockData block;
  glm::vec3 neighbor;
  bool hasNeighbor = false;
};

class WorldRayCast {
  bool successful;
  HitTarget hitTarget;

public:
  WorldRayCast(glm::vec3 position, glm::vec3 direction, World& world, float reach);

  [[nodiscard]] bool hasHit() const { return successful; };
  [[nodiscard]] HitTarget getHitTarget() const { return hitTarget; };
  explicit operator bool() const { return hasHit(); }
};
