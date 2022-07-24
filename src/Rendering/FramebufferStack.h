#ifndef FRAMEBUFFERSTACK_H
#define FRAMEBUFFERSTACK_H

#include "../glCraft.h"
#include "FrameBuffer.h"

class FramebufferStack {
  std::vector<Ref<Framebuffer>> stack;

public:
  void push(const Ref<Framebuffer>& framebuffer);
  [[nodiscard]] Ref<Framebuffer> peek() const;
  Ref<Framebuffer> pop();

  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;
};

#endif
