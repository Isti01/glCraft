#include "AABB.h"

float AABB::getMinAt(int32_t index) const {
  float min = (&points[0].x)[index];

  for (int32_t i = 1; i < points.size(); i++) {
    float val = (&points[i].x)[index];
    if (val < min) {
      min = val;
    }
  }

  return min;
}

float AABB::getMaxAt(int32_t index) const {
  float max = (&points[0].x)[index];

  for (int32_t i = 1; i < points.size(); i++) {
    float val = (&points[i].x)[index];
    if (val > max) {
      max = val;
    }
  }

  return max;
}

bool AABB::intersect(const AABB& aabb) const {
  return (getMinX() <= aabb.getMaxX() && getMaxX() >= aabb.getMinX()) &&
         (getMinY() <= aabb.getMaxY() && getMaxY() >= aabb.getMinY()) &&
         (getMinZ() <= aabb.getMaxZ() && getMaxZ() >= aabb.getMinZ());
}
