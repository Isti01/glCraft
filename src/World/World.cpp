#include "World.h"

#include "../Util/Math.h"

World::World() {
  defaultShader = AssetManager::instance().loadShaderProgram("assets/shaders/default");
  textureAtlas = AssetManager::instance().loadTexture("assets/textures/default_texture.png");
  defaultShader->setTexture("atlas", textureAtlas, 0);
}

void World::render(glm::vec3 playerPos, glm::mat4 transform) {
  // todo sort the chunks before rendering

  for (auto& [position, chunk]: chunks) {
    chunk->render(transform * glm::translate(glm::vec3(position.x, 0, position.y)));
  }
}

BlockData World::getBlockAt(glm::ivec3 position) {
  return getChunk(getChunkIndex(position))->getBlockAt(Chunk::toChunkCoordinates(position));
}

bool World::isValidBlockPosition(glm::ivec3 position) {
  return Chunk::isValidPosition(position);
}

bool World::placeBlock(BlockData block, glm::ivec3 position) {
  if (!Chunk::isValidPosition(position)) return false;

  Ref<Chunk> chunk = getChunk(getChunkIndex(position));
  chunk->placeBlock(block, Chunk::toChunkCoordinates(position));
  glm::ivec2 localPosition = position % Chunk::HorizontalSize;

  return true;
}

glm::ivec2 World::getChunkIndex(glm::ivec3 position) {
  return {position.x - Math::positiveMod(position.x, Chunk::HorizontalSize),
          position.z - Math::positiveMod(position.z, Chunk::HorizontalSize)};
}

Ref<Chunk> World::getChunk(glm::ivec2 position) {
  if (!chunks.contains(position)) { addChunk(position, generateOrLoadChunk(position)); }

  return chunks.at(position);
}
