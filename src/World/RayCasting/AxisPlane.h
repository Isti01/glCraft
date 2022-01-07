#pragma once

#include "../../glCraft.h"

class AxisPlane {
  glm::vec3 plane;

  float offsetDirection;
  float offset;

  glm::vec3 rayPosition;
  glm::vec3 rayDirection;

  glm::vec3 hitPosition;
  float hitDistance;
  glm::ivec3 hitBlockPosition;

  [[nodiscard]] inline float intersect() const;

  [[nodiscard]] float calculateOffsetDirection(const glm::vec3& direction) const;
  [[nodiscard]] float calculateStartOffset(const glm::vec3& position, const glm::vec3& direction) const;

  [[nodiscard]] float calculateHitDistanceToPosition() const { return glm::distance(rayPosition, hitPosition); }
  [[nodiscard]] glm::vec3 calculateHitPosition() const;
  [[nodiscard]] glm::ivec3 calculateBlockPosition();

public:
  AxisPlane(glm::vec3 plane, glm::vec3 rayPosition, glm::vec3 rayDirection);

  [[nodiscard]] glm::vec3 getHitPosition() const { return hitPosition; };
  [[nodiscard]] float getHitDistance() const { return hitDistance; };
  [[nodiscard]] glm::ivec3 getBlockPosition() const { return hitBlockPosition; };


  bool operator<(const AxisPlane& other) const { return hitDistance < other.hitDistance; }

  void advanceOffset();
};
