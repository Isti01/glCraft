#pragma once

#include "../glCraft.h"

class Renderbuffer {
  uint32_t id = 0;
  int32_t width;
  int32_t height;

public:
  Renderbuffer(uint32_t type, int32_t width, int32_t height);
  ~Renderbuffer();

  [[nodiscard]] int32_t getWidth() const { return width; };
  [[nodiscard]] int32_t getHeight() const { return height; };
  [[nodiscard]] bool isValid() const { return id != 0; };
  [[nodiscard]] uint32_t getId() const { return id; };

  void bind();
  void unbind();
};
