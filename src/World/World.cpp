#include "World.h"

#include <ranges>

#include "../Application/Window.h"
#include "../AssetManager/AssetManager.h"
#include "../Rendering/ColorRenderPass.h"
#include "../Rendering/FullscreenQuad.h"

World::World(const Ref<Persistence>& persistence, std::vector<Ref<WorldBehavior>> behaviors, int32_t seed)
    : behaviors(std::move(behaviors)),
      persistence(persistence),
      generator(seed) {
  TRACE_FUNCTION();
  opaqueShader = AssetManager::instance().loadShaderProgram("assets/shaders/world_opaque");
  transparentShader = AssetManager::instance().loadShaderProgram("assets/shaders/world_transparent");
  blendShader = AssetManager::instance().loadShaderProgram("assets/shaders/world_blend");
  setTextureAtlas(AssetManager::instance().loadTextureArray("assets/textures/default_texture.png"));
}

Ref<Chunk> World::generateOrLoadChunk(glm::ivec2 position) {
  TRACE_FUNCTION();
  Ref<Chunk> chunk = persistence->getChunk(position);
  if (chunk != nullptr) {
    return chunk;
  }
  chunk = std::make_shared<Chunk>(position);
  generator.populateChunk(chunk);
  persistence->commitChunk(chunk);

  return chunk;
}

void World::unloadChunk(const Ref<Chunk>& chunk) {
  const auto chunkPos = chunk->getPosition();
  chunks.erase(chunkPos);
  for (int32_t x = 0; x < Chunk::HorizontalSize; ++x) {
    for (int32_t y = 0; y < Chunk::VerticalSize; ++y) {
      for (int32_t z = 0; z < Chunk::HorizontalSize; ++z) {
        for (const auto& worldBehavior: behaviors) {
          glm::ivec3 blockPos = {x, y, z};
          glm::ivec3 globalBlockPos = blockPos + glm::ivec3(chunkPos.x, 0, chunkPos.y);
          worldBehavior->onBlockRemoved(globalBlockPos, chunk->getBlockAt(blockPos), *this, false);
        }
      }
    }
  }
}

void World::update(const glm::vec3& playerPosition, float deltaTime) {
  TRACE_FUNCTION();
  textureAnimation += deltaTime * TextureAnimationSpeed;

  glm::vec2 playerChunkPosition = getChunkIndex(playerPosition);

  auto chunksCopy = chunks;
  float unloadDistance = static_cast<float>(viewDistance + 1) * 16 + 8.0f;
  for (const auto& [chunkPosition, chunk]: chunksCopy) {
    if (glm::abs(glm::distance(glm::vec2(chunkPosition), playerChunkPosition)) > unloadDistance) {
      unloadChunk(chunk);
    }
  }

  float loadDistance = static_cast<float>(viewDistance) * 16 + 8.0f;
  for (int32_t i = -viewDistance; i <= viewDistance; i++) {
    for (int32_t j = -viewDistance; j <= viewDistance; j++) {
      glm::ivec2 position = glm::ivec2(i * 16, j * 16) + glm::ivec2(playerChunkPosition);
      if (isChunkLoaded(position)) {
        continue;
      }

      float distance = glm::abs(glm::distance(glm::vec2(position), playerChunkPosition));
      if (distance <= loadDistance) {
        addChunk(position, generateOrLoadChunk(position));
      }
    }
  }

  for (auto& behavior: behaviors) {
    behavior->update(deltaTime);
  }
}

void World::sortChunkIndices(glm::vec3 playerPos, const Ref<ChunkIndexVector>& chunkIndices) {
  chunkIndices->clear();
  if (chunkIndices->capacity() < chunks.size()) {
    chunkIndices->reserve(chunks.size());
  }

  glm::vec2 playerXZ = glm::vec2(playerPos.x, playerPos.z);
  for (const auto& [key, value]: chunks) {
    chunkIndices->emplace_back(key, value->distanceToPoint(playerXZ));
  }

  std::sort(chunkIndices->begin(), chunkIndices->end(),
            [](const auto& a, const auto& b) { return b.second < a.second; });
}

void World::rebuildChunks(const Ref<ChunkIndexVector>& chunkIndices, const Frustum& frustum) {
  uint32_t meshesRebuilt = 0;
  for (auto& index: std::ranges::reverse_view(*chunkIndices)) {
    if (meshesRebuilt > MaxRebuildsAllowedPerFrame) {
      break;
    }
    const auto& chunk = chunks[index.first];
    if (chunk->needsMeshRebuild() && chunk->isVisible(frustum)) {
      chunk->rebuildMesh(*this);
      meshesRebuilt++;
    }
  }
}

void World::renderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum) {
  TRACE_FUNCTION();
  static auto sortedChunkIndices = std::make_shared<ChunkIndexVector>();
  sortChunkIndices(playerPos, sortedChunkIndices);
  rebuildChunks(sortedChunkIndices, frustum);

  const int32_t animationProgress = static_cast<int32_t>(textureAnimation) % 5;

  // animation offsets for water and lava
  const static int32_t animationOffsets[] = {0, 1, 2, 17, 18};

  const int32_t animationOffset = animationOffsets[animationProgress];

  opaqueShader->setUInt("textureAnimation", animationOffset);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (const auto& index: *sortedChunkIndices) {
    const auto& chunk = chunks[index.first];
    chunk->setShader(opaqueShader);
    chunk->setUseAmbientOcclusion(useAmbientOcclusion);
    chunk->renderOpaque(transform, frustum);
    chunk->renderSemiTransparent(transform, frustum);
  }

  for (const auto& behavior: behaviors) {
    behavior->renderOpaque(transform, playerPos, frustum);
  }

  glDisable(GL_BLEND);
}

/// implemented this paper: https://jcgt.org/published/0002/02/09/
void World::renderTransparent(glm::mat4 transform,
                              glm::vec3 playerPos,
                              const Frustum& frustum,
                              float zNear,
                              float zFar,
                              const Ref<Framebuffer>& opaqueRender) {
  TRACE_FUNCTION();
  auto width = opaqueRender->getWidth();
  auto height = opaqueRender->getHeight();
  static Ref<Framebuffer> framebuffer = nullptr;
  if (framebuffer == nullptr || framebuffer->getWidth() != width || framebuffer->getHeight() != height) {
    framebuffer = std::make_shared<Framebuffer>(width, height, false, 2);
  }
  static auto sortedChunkIndices = std::make_shared<ChunkIndexVector>();
  sortChunkIndices(playerPos, sortedChunkIndices);
  rebuildChunks(sortedChunkIndices, frustum);

  const int32_t animationProgress = static_cast<int32_t>(textureAnimation) % 5;

  // animation offsets for water and lava
  const static int32_t animationOffsets[] = {0, 1, 2, 17, 18};
  const int32_t animationOffset = animationOffsets[animationProgress];
  auto& window = Window::instance();
  window.getFramebufferStack()->push(framebuffer);
  glEnable(GL_BLEND);

  glBlendFunci(0, GL_ONE, GL_ONE);
  glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  framebuffer->clearColorAttachment(1, glm::vec4(1));

  transparentShader->setUInt("textureAnimation", animationOffset);
  transparentShader->setFloat("zNear", zNear);
  transparentShader->setFloat("zFar", zFar);
  transparentShader->setTexture("opaqueDepth", opaqueRender->getDepthAttachment(), 1);
  transparentShader->bind();

  for (const auto& [key, chunk]: chunks) {
    chunk->setShader(transparentShader);
    chunk->setUseAmbientOcclusion(useAmbientOcclusion);
    chunk->renderSemiTransparent(transform, frustum);
  }
  Window::instance().getFramebufferStack()->pop();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ColorRenderPass renderPass(blendShader);
  renderPass.setTexture("accumTexture", framebuffer->getColorAttachment(0), 1);
  renderPass.setTexture("revealageTexture", framebuffer->getColorAttachment(1), 2);
  renderPass.setTexture("opaqueTexture", opaqueRender->getColorAttachment(0), 3);
  renderPass.render();
  glDisable(GL_BLEND);
}

const BlockData* World::getBlockAt(glm::ivec3 position) {
  return getChunk(getChunkIndex(position))->getBlockAt(Chunk::toChunkCoordinates(position));
}
bool World::isValidBlockPosition(glm::ivec3 position) {
  return Chunk::isValidPosition(position);
}


bool World::placeBlock(BlockData block, glm::ivec3 position) {
  TRACE_FUNCTION();
  if (!Chunk::isValidPosition(position)) {
    return false;
  }

  glm::ivec3 positionInChunk = Chunk::toChunkCoordinates(position);
  auto chunk = getChunk(getChunkIndex(position));
  auto oldBlock = chunk->getBlockAt(positionInChunk);
  for (const auto& behavior: behaviors) {
    behavior->onBlockRemoved(position, oldBlock, *this, true);
  }

  getChunk(getChunkIndex(position))->placeBlock(block, positionInChunk);
  for (const auto& behavior: behaviors) {
    behavior->onNewBlock(position, &block, *this);
  }

  constexpr std::array<glm::ivec3, 6> blocksAround = {
     {{0, 0, 1}, {1, 0, 0}, {0, 0, -1}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}}};
  for (const glm::ivec3& offset: blocksAround) {
    glm::ivec3 neighbor = offset + positionInChunk;
    glm::ivec3 neighborWorldPosition = position + offset;
    if (!Chunk::isInBounds(neighbor.x, neighbor.y, neighbor.z)) {
      const auto& chunk = getChunk(getChunkIndex(neighborWorldPosition));
      chunk->setDirty();
    }
    for (const auto& behavior: behaviors) {
      behavior->onBlockUpdate(neighborWorldPosition, getBlockAt(neighborWorldPosition), *this);
    }
  }

  return true;
}

glm::ivec2 World::getChunkIndex(glm::ivec3 position) {
  return {position.x - Util::positiveMod(position.x, Chunk::HorizontalSize),
          position.z - Util::positiveMod(position.z, Chunk::HorizontalSize)};
}

Ref<Chunk> World::getChunk(glm::ivec2 position) {
  TRACE_FUNCTION();
  if (!isChunkLoaded(position)) {
    addChunk(position, generateOrLoadChunk(position));
  }

  return chunks.at(position);
}

void World::addChunk(glm::ivec2 position, const Ref<Chunk>& chunk) {
  TRACE_FUNCTION();
  chunks[position] = chunk;
  std::array<glm::ivec2, 4> chunksAround = {{{0, 16}, {16, 0}, {0, -16}, {-16, 0}}};
  for (const glm::ivec2& offset: chunksAround) {
    glm::ivec2 neighborPosition = position + offset;

    if (!isChunkLoaded(neighborPosition))
      continue;

    chunks[neighborPosition]->setDirty();
  }
  for (int32_t x = 0; x < Chunk::HorizontalSize; ++x) {
    for (int32_t y = 0; y < Chunk::VerticalSize; ++y) {
      for (int32_t z = 0; z < Chunk::HorizontalSize; ++z) {
        for (const auto& worldBehavior: behaviors) {
          glm::ivec3 blockPos = {x, y, z};
          worldBehavior->onNewBlock(blockPos + glm::ivec3(position.x, 0, position.y), chunk->getBlockAt(blockPos),
                                    *this);
        }
      }
    }
  }
}

void World::setTextureAtlas(const Ref<const Texture>& texture) {
  textureAtlas = texture;
  opaqueShader->setTexture("atlas", textureAtlas, 0);
  transparentShader->setTexture("atlas", textureAtlas, 0);
}
const BlockData* World::getBlockAtIfLoaded(glm::ivec3 position) const {
  glm::ivec2 index = getChunkIndex(position);
  if (!isChunkLoaded(index)) {
    return nullptr;
  }

  return chunks.at(index)->getBlockAt(Chunk::toChunkCoordinates(position));
}
bool World::isChunkLoaded(glm::ivec2 position) const {
  return chunks.contains(position);
}
