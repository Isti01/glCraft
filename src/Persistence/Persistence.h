#pragma once

#include "../Scene/Camera.h"
#include "../Util/Util.h"
#include "../World/Chunk.h"
#include "../glCraft.h"

class Persistence {
  std::string path;
  Camera camera;
  std::unordered_map<glm::ivec2, Ref<Chunk>, Util::HashVec2> chunks;

public:
  explicit Persistence(std::string path);

  void commitChunk(const Ref<Chunk>& chunk) { chunks[chunk->getPosition()] = chunk; }
  [[nodiscard]] Ref<Chunk> getChunk(glm::ivec2 position) const {
    if (!chunks.contains(position)) {
      return nullptr;
    }
    return chunks.at(position);
  };

  void commitCamera(const Camera& newCamera) { camera = newCamera; }
  [[nodiscard]] const Camera& getCamera() const { return camera; };

  ~Persistence();
};