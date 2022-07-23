#include "FrameBuffer.h"

Framebuffer::Framebuffer(int32_t width, int32_t height, bool createDepthAttachment, int32_t colorAttachmentCount)
    : width(width),
      height(height) {
  glGenFramebuffers(1, &id);
  bind(false);

  if (createDepthAttachment) {
    depthAttachment = std::make_shared<Renderbuffer>(GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthAttachment->getId());
  }

  for (uint32_t i = 0; i < colorAttachmentCount; ++i) {
    Ref<Texture> attachment = std::make_shared<Texture>(GL_TEXTURE_2D, false, 0);
    attachment->allocateTexture(width, height);

    uint32_t attachmentName = GL_COLOR_ATTACHMENT0 + i;
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentName, GL_TEXTURE_2D, attachment->getId(), 0);

    attachmentNames.push_back(attachmentName);
    attachments.push_back(attachment);
  }
  unbind();
}

Framebuffer::~Framebuffer() {
  if (id) {
    glDeleteFramebuffers(1, &id);
  }
}

Ref<Renderbuffer> Framebuffer::getDepthAttachment() {
  return depthAttachment;
}

Ref<Texture> Framebuffer::getColorAttachment(int32_t index) {
  assert(index < attachments.size());
  return attachments[index];
}

void Framebuffer::clearColorAttachment(int32_t index, const glm::vec4 &value) {
  bind();
  assert(index < attachments.size());
  glClearBufferfv(GL_COLOR, index, &value.x);
}

void Framebuffer::bind(bool forDrawing) {
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  if (forDrawing) {
    glDrawBuffers(static_cast<int32_t>(attachmentNames.size()), attachmentNames.data());
  }
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
