#pragma once

#include "../../World/BlockData.h"

class World;
struct BlockData;
class WorldBehavior {
public:
  virtual void onNewBlock(glm::ivec3 blockPos, const BlockData *block, World &world) = 0;
  virtual void onBlockUpdate(glm::ivec3 blockPos, const BlockData *block, World &world) = 0;
  virtual void onBlockRemoved(glm::ivec3 blockPos, const BlockData *block, World &world) = 0;

  virtual void update(float dt) = 0;
  virtual void renderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum &frustum) = 0;

  virtual ~WorldBehavior() = default;
};
