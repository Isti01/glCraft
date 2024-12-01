#pragma once
#include "../../Rendering/ParticleSystem.h"
#include "../../World/World.h"
#include "../CubeMesh.h"

class LavaParticleSystem : public ParticleSystem {
  CubeMesh cubeMesh{};
  Ref<const ShaderProgram> cubeShader;

public:
  LavaParticleSystem();
  void render(glm::mat4 MVP) override;
};

class LavaParticleBehavior : public WorldBehavior {
  Random random;
  std::set<glm::ivec3, Util::CompareIVec3> surfaceLavaPositions{};
  LavaParticleSystem particleSystem;
  float emitAttemptFrequency = 0.1;  // seconds
  float timeUntilNextEmit = emitAttemptFrequency;

  void emitLavaParticles(glm::ivec3 pos);
  void emitLavaParticle(glm::vec3 pos);

public:
  LavaParticleBehavior() = default;
  void onNewBlock(glm::ivec3 blockPos, const BlockData *block, World &world) override;
  void onBlockUpdate(glm::ivec3 blockPos, const BlockData *block, World &world) override;
  void onBlockRemoved(glm::ivec3 blockPos, const BlockData *block, World &world, bool removedByPlayer) override;
  void renderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum &frustum) override;
  void update(float dt) override;
};
