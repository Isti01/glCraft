#include "World.h"

#include "../Util/Math.h"

World::World() {
  defaultShader = AssetManager::instance().loadShaderProgram("assets/shaders/default");
  textureAtlas = AssetManager::instance().loadTexture("assets/textures/default_texture.png");
  defaultShader->setTexture("atlas", textureAtlas, 0);
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
  if (!Chunk::isValidPosition(position)) return false;

  getChunk(getChunkIndex(position))->placeBlock(block, Chunk::toChunkCoordinates(position));
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
