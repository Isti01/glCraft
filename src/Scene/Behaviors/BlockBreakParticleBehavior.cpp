#include "BlockBreakParticleBehavior.h"

#include "../../AssetManager/AssetManager.h"

BlockBreakParticleSystem::BlockBreakParticleSystem()
    : cubeShader(AssetManager::instance().loadShaderProgram("assets/shaders/colored_cube_opaque")) {}

void BlockBreakParticleSystem::render(glm::mat4 MVP) {
  cubeShader->bind();

  for (const auto &particle: particles) {
    cubeShader->setVec4("color", particle.color);
    cubeShader->setMat4("MVP", MVP * glm::translate(particle.position) * glm::scale(particle.scale) *
                                  glm::rotate(glm::two_pi<float>(), particle.rotation));
    cubeMesh.render();
  }
}


void BlockBreakParticleBehavior::onBlockRemoved(glm::ivec3 blockPos,
                                                const BlockData *block,
                                                World &world,
                                                bool removedByPlayer) {
  if (!removedByPlayer)
    return;
  if (block == nullptr || block->type == BlockData::BlockType::air)
    return;

  auto color = block->getColor();
  for (int i = 0; i < 50; ++i) {
    emitBlockParticle(glm::vec3(blockPos) + random.getVec3(), color);
  }
}

void BlockBreakParticleBehavior::emitBlockParticle(glm::vec3 pos, glm::vec4 color) {
  particleSystem.emit({
     .position = pos,
     .scale = glm::vec3(0.0625),
     .scaleVelocity = glm::vec3(-0.0625),
     .scaleVelocityVariation = glm::vec3(.05),
     .velocity = glm::vec3(1, 0, 1),
     .velocityVariation = glm::vec3(-2, 1, -2),
     .gravity = glm::vec3(0, -9.8, 0),
     .rotation = glm::vec3(0),
     .angularVelocity = glm::vec3(1, -1, 1),
     .angularVelocityVariation = glm::vec3(.25, .25, .25),
     .startColor = color,
     .endColor = color,
     .startLifetime = .200,
     .lifetimeVariation = .200,
     .lifetime = .200,
  });
}


void BlockBreakParticleBehavior::update(float dt) {
  particleSystem.update(dt);
}

void BlockBreakParticleBehavior::renderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum &frustum) {
  particleSystem.render(transform);
}
