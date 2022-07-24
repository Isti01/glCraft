#include "FramebufferStack.h"

void FramebufferStack::push(const Ref<Framebuffer>& framebuffer) {
  stack.push_back(framebuffer);
  framebuffer->bind();
}

Ref<Framebuffer> FramebufferStack::peek() const {
  return empty() ? nullptr : stack.back();
}

Ref<Framebuffer> FramebufferStack::pop() {
  assert(!empty());

  Ref<Framebuffer> framebuffer = peek();
  stack.pop_back();

  auto current = peek();
  if (current != nullptr) {
    current->bind();
  } else {
    framebuffer->unbind();  // binds the default framebuffer
  }

  return framebuffer;
}

bool FramebufferStack::empty() const {
  return stack.empty();
}

size_t FramebufferStack::size() const {
  return stack.size();
}
