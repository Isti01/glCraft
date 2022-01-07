#include "World.h"

void World::render(glm::vec3 playerPos, glm::mat4 transform) {
  // todo sort the chunks before rendering

  for (auto& [position, chunk]: chunks) {
    chunk->render(transform * glm::translate(glm::vec3(position.x, 0, position.y)));
  }
}
BlockData World::getBlockAt(glm::ivec3 position) {
  return getChunk(getChunkIndex(position))->getBlockAt(Chunk::toChunkCoordinates(position));
}
