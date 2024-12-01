#pragma once

#include <Frustum.h>

#include "../../World/BlockData.h"

class World;
struct BlockData;
class WorldBehavior {
public:
  virtual void onNewBlock(glm::ivec3 blockPos, const BlockData *block, World &world) {}
  virtual void onBlockUpdate(glm::ivec3 blockPos, const BlockData *block, World &world) {}
  virtual void onBlockRemoved(glm::ivec3 blockPos, const BlockData *block, World &world, bool removedByPlayer) {}

  virtual void update(float dt) {}
  virtual void renderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum &frustum) {}

  virtual ~WorldBehavior() = default;
};
