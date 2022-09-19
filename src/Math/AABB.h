#pragma once

#include "../glCraft.h"

struct AABB {
  glm::vec3 minPoint;
  glm::vec3 maxPoint;

  explicit constexpr AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
      : minPoint(minPoint),
        maxPoint(maxPoint) {}

  static AABB fromBlockPosition(const glm::vec3& position) { return AABB(position, position + glm::vec3{1, 1, 1}); }

  [[nodiscard]] bool intersect(const AABB& aabb) const {
    return (getMinX() <= aabb.getMaxX() && getMaxX() >= aabb.getMinX()) &&
           (getMinY() <= aabb.getMaxY() && getMaxY() >= aabb.getMinY()) &&
           (getMinZ() <= aabb.getMaxZ() && getMaxZ() >= aabb.getMinZ());
  }

  [[nodiscard]] glm::vec3 getMinPoint() const { return minPoint; };
  [[nodiscard]] glm::vec3 getMaxPoint() const { return maxPoint; };

  [[nodiscard]] float getMinX() const { return minPoint.x; };
  [[nodiscard]] float getMinY() const { return minPoint.y; };
  [[nodiscard]] float getMinZ() const { return minPoint.z; };
  [[nodiscard]] float getMaxX() const { return maxPoint.x; };
  [[nodiscard]] float getMaxY() const { return maxPoint.y; };
  [[nodiscard]] float getMaxZ() const { return maxPoint.z; };
};