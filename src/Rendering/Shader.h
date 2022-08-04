#pragma once

#include "../glCraft.h"

class Shader {
  uint32_t id;

public:
  Shader(const std::string& source, uint32_t type);
  explicit Shader(const std::string &name);
  ~Shader();

  [[nodiscard]] bool isValid() const { return id != 0; };
  [[nodiscard]] uint32_t getId() const { return id; };

  Shader(const Shader &) = delete;
  Shader(Shader &) = delete;
  Shader(Shader &&) noexcept = delete;
  Shader &operator=(Shader &) = delete;
  Shader &operator=(Shader &&) noexcept = delete;
};
