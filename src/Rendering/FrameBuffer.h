#pragma once

#include "../glCraft.h"
#include "Texture.h"

class Framebuffer {
  uint32_t id = 0;

  std::vector<Ref<Texture>> attachments;
  std::vector<uint32_t> attachmentNames;
  Ref<Texture> depthAttachment = nullptr;

  int32_t width;
  int32_t height;

public:
  Framebuffer(int32_t width, int32_t height, bool createDepthAttachment, int32_t colorAttachmentCount);
  ~Framebuffer();

  [[nodiscard]] int32_t getWidth() const { return width; };
  [[nodiscard]] int32_t getHeight() const { return height; };
  [[nodiscard]] bool isValid() const { return id != 0; };
  [[nodiscard]] size_t getAttachmentCount() const { return attachments.size(); };

  Ref<Texture> getDepthAttachment();
  Ref<Texture> getColorAttachment(int32_t index);

  void clearColorAttachment(int32_t index, const glm::vec4 &value);

  void bind(bool forDrawing = true);
  void unbind();

  Framebuffer(const Framebuffer &) = delete;
  Framebuffer(Framebuffer &) = delete;
  Framebuffer(Framebuffer &&) noexcept = delete;
  Framebuffer &operator=(Framebuffer &) = delete;
  Framebuffer &operator=(Framebuffer &&) noexcept = delete;
};
