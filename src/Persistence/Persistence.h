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
  ~Persistence();

  void commitChunk(const Ref<Chunk>& chunk);
  [[nodiscard]] Ref<Chunk> getChunk(glm::ivec2 position) const;

  void commitCamera(const Camera& newCamera);
  [[nodiscard]] const Camera& getCamera() const;
};