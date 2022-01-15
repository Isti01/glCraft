#include "World.h"

#include "../Util/Math.h"

World::World(int32_t seed) : generator(seed) {
  defaultShader = AssetManager::instance().loadShaderProgram("assets/shaders/default");
  setTextureAtlas(AssetManager::instance().loadTexture("assets/textures/default_texture.png"));
}

Ref<Chunk> World::generateOrLoadChunk(glm::ivec2 position) {
  Ref<Chunk> chunk = std::make_shared<Chunk>(position);
  generator.populateChunk(chunk);

  return chunk;  // todo make the chunks persistent
}

void World::update(const glm::vec3& playerPosition) {
  glm::vec2 playerChunkPosition = getChunkIndex(playerPosition);

  auto chunksCopy = chunks;
  float unloadDistance = static_cast<float>(viewDistance + 1) * 16 + 8.0f;
  for (const auto& [chunkPosition, chunk]: chunksCopy) {
    if (glm::abs(glm::distance(glm::vec2(chunkPosition), playerChunkPosition)) > unloadDistance) {
      chunks.erase(chunkPosition);
    }
  }

  float loadDistance = static_cast<float>(viewDistance) * 16 + 8.0f;
  for (int32_t i = -viewDistance; i < viewDistance; i++) {
    for (int32_t j = -viewDistance; j < viewDistance; j++) {
      glm::ivec2 position = glm::ivec2(i * 16, j * 16) + glm::ivec2(playerChunkPosition);
      if (chunks.contains(position)) {
        continue;
      }

      float distance = glm::abs(glm::distance(glm::vec2(position), playerChunkPosition));
      if (distance <= loadDistance) {
        chunks[position] = generateOrLoadChunk(position);
      }
    }
  }
}

void World::render(glm::vec3 playerPos, glm::mat4 transform) {
  // todo sort the chunks before rendering

  for (auto& [position, chunk]: chunks) { chunk->render(transform); }
}

BlockData World::getBlockAt(glm::ivec3 position) {
  return getChunk(getChunkIndex(position))->getBlockAt(Chunk::toChunkCoordinates(position));
}

bool World::isValidBlockPosition(glm::ivec3 position) {
  return Chunk::isValidPosition(position);
}

bool World::placeBlock(BlockData block, glm::ivec3 position) {
  if (!Chunk::isValidPosition(position)) {
    return false;
  }

  getChunk(getChunkIndex(position))->placeBlock(block, Chunk::toChunkCoordinates(position));
  return true;
}
glm::ivec2 World::getChunkIndex(glm::ivec3 position) {
  return {position.x - Math::positiveMod(position.x, Chunk::HorizontalSize),
          position.z - Math::positiveMod(position.z, Chunk::HorizontalSize)};
}


Ref<Chunk> World::getChunk(glm::ivec2 position) {
  if (!chunks.contains(position)) {
    addChunk(position, generateOrLoadChunk(position));
  }

  return chunks.at(position);
}

void World::setTextureAtlas(const Ref<const Texture>& texture) {
  textureAtlas = texture;
  defaultShader->setTexture("atlas", textureAtlas, 0);
}
