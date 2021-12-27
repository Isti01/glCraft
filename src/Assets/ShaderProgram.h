#pragma once
#include "../glCraft.h"

struct VertexAttributeLocation {
  uint32_t location;
  std::string name;
};


class ShaderProgram {
  uint32_t shaderProgram = 0;

  [[nodiscard]] int32_t getUniformLocation(const std::string &location) const;

public:
  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram(ShaderProgram &) = delete;
  ShaderProgram(ShaderProgram &&) = delete;

  ShaderProgram(const std::string &name, const std::vector<VertexAttributeLocation> &attributes);

  [[nodiscard]] bool isValid() const { return shaderProgram; };
  void bind() const;

  void setInt(const std::string &location, int32_t value);
  void setFloat(const std::string &location, float value);
  void setVec3(const std::string &location, const glm::vec3 &value);
  void setMat4(const std::string &location, const glm::mat4 &value);

  ~ShaderProgram();
};
