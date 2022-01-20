#pragma once

#include "../World/World.h"
#include "../glCraft.h"


class MovementSimulation {
private:
  MovementSimulation() = default;

public:
  static bool canMove(const glm::vec3 &from, const glm::vec3 &to, World &world);
};