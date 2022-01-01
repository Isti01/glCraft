#include "Texture.h"

Texture::Texture() {
  glGenTextures(1, &id);
  bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  unbind();
}

void Texture::bufferRGBAData(uint32_t width, uint32_t height, const std::vector<uint8_t>& data) {
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int32_t>(width), static_cast<int32_t>(height), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, &data[0]);
  glGenerateMipmap(GL_TEXTURE_2D);
  unbind();
}

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
  if (isValid()) { glDeleteTextures(1, &id); }
}
Ref<Texture> Texture::createRef() {
  return std::make_shared<Texture>();
}
