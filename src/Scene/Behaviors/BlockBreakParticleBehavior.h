#pragma once
#include "../../Rendering/ParticleSystem.h"
#include "../../World/World.h"
#include "../CubeMesh.h"

class BlockBreakParticleSystem : public ParticleSystem {
  CubeMesh cubeMesh{};
  Ref<const ShaderProgram> cubeShader;

public:
  BlockBreakParticleSystem();
  void render(glm::mat4 MVP) override;
};

class BlockBreakParticleBehavior : public WorldBehavior {
  Random random;
  BlockBreakParticleSystem particleSystem;

  void emitBlockParticle(glm::vec3 pos, glm::vec4 color);

public:
  void onBlockRemoved(glm::ivec3 blockPos, const BlockData *block, World &world, bool removedByPlayer) override;
  void update(float dt) override;
  void renderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum &frustum) override;
};
