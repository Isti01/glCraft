#pragma once

#include "../World/World.h"
#include "../glCraft.h"

namespace MovementSimulation {
  bool canMove(const glm::vec3 &from, const glm::vec3 &to, World &world);
};
