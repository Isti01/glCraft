#pragma once

#include "../Util/Random.h"
#include "../glCraft.h"

struct ParticleDescription {
  glm::vec3 position{0, 0, 0};

  glm::vec3 scale{1, 1, 1};
  glm::vec3 scaleVelocity{1, 1, 1};
  glm::vec3 scaleVelocityVariation{1, 1, 1};

  glm::vec3 velocity{0, 1, 0};
  glm::vec3 velocityVariation{1, 1, 1};
  glm::vec3 gravity{0, -9.81, 0};

  glm::vec3 rotation{0, 0, 0};
  glm::vec3 angularVelocity{0, 0, 0};
  glm::vec3 angularVelocityVariation{0, 0, 0};

  glm::vec4 startColor{1, 1, 1, 1};
  glm::vec4 endColor{.5, .5, .5, 1};

  glm::vec4 color;
  float startLifetime;
  float lifetimeVariation{.5};
  float lifetime{1};
};

class ParticleSystem {
protected:
  Random random;
  std::vector<ParticleDescription> particles;

public:
  ParticleSystem() = default;

  void update(float deltaTime);
  virtual void render(glm::mat4 MVP) = 0;
  void emit(const ParticleDescription& particle);

  virtual ~ParticleSystem() = default;
};
