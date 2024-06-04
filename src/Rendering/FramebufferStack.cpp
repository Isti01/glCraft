#include "FramebufferStack.h"

void FramebufferStack::push(const Ref<Framebuffer>& framebuffer) {
  stack.push_back(framebuffer);
  framebuffer->bind();

  if (keepIntermediateTextures) {
    for (int i = 0; i < framebuffer->getAttachmentCount(); ++i) {
      intermediateTextures.push_back(framebuffer->getColorAttachment(i));
    }
  }
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

void FramebufferStack::clearIntermediateTextureReferences() {
  intermediateTextures.clear();
}

void FramebufferStack::setKeepIntermediateTextures(bool keepBuffers) {
  keepIntermediateTextures = keepBuffers;
}

std::vector<Ref<Texture>> FramebufferStack::getIntermediateTextures() const {
  return intermediateTextures;
}

bool FramebufferStack::empty() const {
  return stack.empty();
}

size_t FramebufferStack::size() const {
  return stack.size();
}
