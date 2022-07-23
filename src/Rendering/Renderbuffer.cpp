#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(uint32_t type, int32_t width, int32_t height) : width(width), height(height) {
  glGenRenderbuffers(1, &id);
  bind();

  glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);

  unbind();
}

Renderbuffer::~Renderbuffer() {
  if (isValid()) {
    glDeleteRenderbuffers(1, &id);
  }
}

void Renderbuffer::bind() {
  glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void Renderbuffer::unbind() {
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
