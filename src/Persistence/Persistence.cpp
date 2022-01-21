#include "Persistence.h"

#include <cstdlib>

Persistence::Persistence(std::string path) : path(std::move(path)) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open the file: " << path << std::endl;
    return;
  }

  file.seekg(0, std::ios::end);
  auto length = size_t(file.tellg());
  if (length == -1) {
    std::cerr << "Failed to read the file: " << path << std::endl;
    return;
  }

  file.seekg(0, std::ios::beg);

  file.read(reinterpret_cast<char*>(&camera), sizeof(camera));
  size_t chunkCount = (length - sizeof(Camera)) / (sizeof(glm::ivec2) + sizeof(Chunk::data));

  for (size_t i = 0; i < chunkCount; i++) {
    glm::ivec2 worldPosition;
    file.read(reinterpret_cast<char*>(&worldPosition[0]), sizeof(glm::ivec2));

    Ref<Chunk> chunk = std::make_shared<Chunk>(worldPosition);
    file.read(reinterpret_cast<char*>(&chunk->data[0]), sizeof(Chunk::data));

    chunks[worldPosition] = chunk;
  }
}

Persistence::~Persistence() {
  std::ofstream file(path, std::ios::out | std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open the file: " << path << std::endl;
    return;
  }

  file.write(reinterpret_cast<char*>(&camera), sizeof(camera));

  for (auto& [key, chunk]: chunks) {
    glm::ivec2 worldPosition = key;
    file.write(reinterpret_cast<char*>(&worldPosition[0]), sizeof(glm::ivec2));
    file.write(reinterpret_cast<char*>(&chunk->data[0]), sizeof(Chunk::data));
  }
}
