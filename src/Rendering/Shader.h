#pragma once
#include "../glCraft.h"

class Shader {
  uint32_t id;

public:
  Shader(const Shader &) = delete;
  Shader(Shader &) = delete;
  Shader(Shader &&) = delete;

  explicit Shader(const std::string &name);
  [[nodiscard]] bool isValid() const { return id != 0; };
  [[nodiscard]] uint32_t getId() const { return id; };
  ~Shader();
};
