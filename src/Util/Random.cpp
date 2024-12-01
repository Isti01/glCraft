#include "Random.h"

Random::Random() : randomEngine(std::random_device()()) {}

float Random::getFloat() {
  return distribution(randomEngine) / static_cast<float>(std::numeric_limits<std::mt19937::result_type>::max());
}
glm::vec2 Random::getVec2() {
  return glm::vec2(getFloat(), getFloat());
}

glm::vec3 Random::getVec3() {
  return glm::vec3(getFloat(), getFloat(), getFloat());
}