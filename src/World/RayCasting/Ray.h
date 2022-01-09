#pragma once

#include "../../glCraft.h"
#include "../World.h"

struct HitTarget {
  glm::vec3 position;
  BlockData block;
  glm::vec3 neighbor;
  bool hasNeighbor = false;
};

class Ray {
  bool successful;
  HitTarget hitTarget;

public:
  Ray(glm::vec3 position, glm::vec3 direction, World& world, float reach);

  [[nodiscard]] bool hasHit() const { return successful; };
  [[nodiscard]] HitTarget getHitTarget() const { return hitTarget; };
  explicit operator bool() const { return hasHit(); }
};
