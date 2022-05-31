#include "AxisPlane.h"

AxisPlane::AxisPlane(glm::vec3 planeNormal, glm::vec3 rayPosition, glm::vec3 rayDirection)
    : planeNormal(planeNormal),
      rayPosition(rayPosition),
      rayDirection(rayDirection) {
  offsetDirection = calculateOffsetDirection(rayDirection);
  planeOffset = calculateStartOffset(rayPosition, rayDirection);
  hitPosition = calculateHitPosition();
  hitDistance = calculateHitDistanceToPosition();
}

glm::vec3 AxisPlane::calculateHitPosition() const {
  float t = intersect();

  // if the intersection result is less than 0 then it either did not hit or the hit position is behind the cast position
  if (t < 0)
    return glm::vec3(std::numeric_limits<float>::infinity());

  return rayPosition + t * rayDirection;
}

float AxisPlane::intersect() const {
  float d = glm::dot(planeNormal, rayDirection);
  if (d == 0)
    return -std::numeric_limits<float>::infinity();  // the plane and the ray are parallel

  float t = glm::dot(planeNormal, planeNormal * planeOffset - rayPosition);
  float td = t / d;
  return td;
}

float AxisPlane::calculateOffsetDirection(const glm::vec3& direction) const {
  return glm::dot(planeNormal, direction) < 0 ? -1.0f : 1.0f;
}

float AxisPlane::calculateStartOffset(const glm::vec3& position, const glm::vec3& direction) const {
  return std::floor(glm::dot(planeNormal, position)) + (glm::dot(planeNormal, direction) > 0 ? 1.0f : 0.0f);
}

void AxisPlane::advanceOffset() {
  planeOffset += offsetDirection;
  hitPosition = calculateHitPosition();
  hitDistance = calculateHitDistanceToPosition();
}

std::optional<glm::ivec3> AxisPlane::rayHitsToBlockPosition(const glm::vec3& hit1, const glm::vec3& hit2) {
  glm::vec3 diff = glm::abs(hit1 - hit2);

  if (diff.x > 1.001f || diff.y > 1.001f || diff.z > 1.001f)
    return std::nullopt;
  return glm::floor((hit1 + hit2) / 2.0f);
}