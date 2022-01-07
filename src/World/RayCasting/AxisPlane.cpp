#include "AxisPlane.h"

AxisPlane::AxisPlane(glm::vec3 plane, glm::vec3 rayPosition, glm::vec3 rayDirection)
    : plane(plane),
      offsetDirection(calculateOffsetDirection(rayDirection)),
      offset(calculateStartOffset(rayPosition, rayDirection)),
      rayPosition(rayPosition),
      rayDirection(rayDirection),
      hitPosition(calculateHitPosition()),
      hitDistance(calculateHitDistanceToPosition()),
      hitBlockPosition(calculateBlockPosition()) {}

glm::vec3 AxisPlane::calculateHitPosition() const {
  float t = intersect();

  // if the intersection result is less than 0 then it either did not hit or the hit position is behind the cast position
  if (t < 0) return glm::vec3(std::numeric_limits<float>::infinity());

  return rayPosition + t * rayDirection;
}

float AxisPlane::intersect() const {
  float d = plane.x * rayDirection.x + plane.y * rayDirection.y + plane.z * rayDirection.z;
  if (d == 0) return -std::numeric_limits<float>::infinity();  // the plane and the ray are parallel

  return -((rayPosition.x + rayPosition.y + rayPosition.z + offset) / d);
}

float AxisPlane::calculateOffsetDirection(const glm::vec3& direction) const {
  return glm::dot(plane, direction) < 0 ? -1.0f : 1.0f;
}

float AxisPlane::calculateStartOffset(const glm::vec3& position, const glm::vec3& direction) const {
  return std::floor(glm::dot(plane, position)) + (glm::dot(plane, direction) > 0 ? 1.0f : 0.0f);
}

void AxisPlane::advanceOffset() {
  offset += offsetDirection;
  hitPosition = calculateHitPosition();
  hitDistance = calculateHitDistanceToPosition();
  hitBlockPosition = calculateBlockPosition();
}

glm::ivec3 AxisPlane::calculateBlockPosition() {
  return hitPosition + glm::sign(rayDirection);
}
