#pragma once

#include "../glCraft.h"

class AxisPlane {
  glm::vec3 planeNormal;

  float offsetDirection;
  float planeOffset;

  glm::vec3 rayPosition;
  glm::vec3 rayDirection;

  glm::vec3 hitPosition;
  float hitDistance;

  [[nodiscard]] float intersect() const;

  [[nodiscard]] float calculateOffsetDirection(const glm::vec3& direction) const;
  [[nodiscard]] float calculateStartOffset(const glm::vec3& position, const glm::vec3& direction) const;

  [[nodiscard]] float calculateHitDistanceToPosition() const { return glm::distance(rayPosition, hitPosition); }
  [[nodiscard]] glm::vec3 calculateHitPosition() const;

public:
  static std::optional<glm::ivec3> rayHitsToBlockPosition(const glm::vec3& hit1, const glm::vec3& hit2);
  AxisPlane(glm::vec3 planeNormal, glm::vec3 rayPosition, glm::vec3 rayDirection);

  [[nodiscard]] glm::vec3 getHitPosition() const { return hitPosition; };
  [[nodiscard]] float getHitDistance() const { return hitDistance; };

  bool operator<(const AxisPlane& other) const { return hitDistance < other.hitDistance; }

  void advanceOffset();
};
