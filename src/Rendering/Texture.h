#pragma once

#include "../glCraft.h"
class Texture {
  uint32_t id = 0;

public:
  Texture(const Texture&) = delete;
  Texture(Texture&) = delete;
  Texture(Texture&&) = delete;

  Texture();

  [[nodiscard]] bool isValid() const { return id != 0; };

  void bind() const;
  void bindToSlot(uint32_t slot) const;
  void unbind() const;

  void bufferRGBAData(uint32_t width, uint32_t height, const std::vector<uint8_t>& data);

  ~Texture();

  static Ref<Texture> createRef();
};
