#include "ParticleSystem.h"

void ParticleSystem::update(float deltaTime) {
  std::vector<size_t> particlesToRemove;
  for (unsigned int i = 0; i < particles.size(); ++i) {
    particles[i].lifetime -= deltaTime;
    if (particles[i].lifetime < 0) {
      particlesToRemove.push_back(i);
      continue;
    }
    const auto progress = 1 - (particles[i].lifetime / particles[i].startLifetime);
    particles[i].color = glm::mix(particles[i].startColor, particles[i].endColor, progress);

    particles[i].scale += particles[i].scaleVelocity * deltaTime;
    particles[i].velocity += particles[i].gravity * deltaTime;
    particles[i].position += particles[i].velocity * deltaTime;
    particles[i].rotation += particles[i].angularVelocity * deltaTime;
  }

  for (int i = particlesToRemove.size() - 1; i >= 0; --i) {
    particles.erase(particles.begin() + i);
  }
}

void ParticleSystem::emit(const ParticleDescription& particle) {
  auto& newParticle = particles.emplace_back(particle);
  newParticle.lifetime += particle.lifetimeVariation * random.getFloat();
  newParticle.velocity += particle.velocityVariation * random.getVec3();
  newParticle.angularVelocity += particle.angularVelocityVariation * random.getVec3();
  newParticle.scaleVelocity += particle.scaleVelocityVariation * random.getVec3();
  newParticle.color = newParticle.startColor;
}
