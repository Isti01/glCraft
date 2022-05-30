#include "Texture.h"

#include "../AssetManager/AssetManager.h"
#include "Image.h"

Texture::Texture(uint32_t image_type, int32_t maxLod) : type(image_type) {
  assert(type == GL_TEXTURE_2D || type == GL_TEXTURE_CUBE_MAP);
  glGenTextures(1, &id);
  bind();

  glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  if (type == GL_TEXTURE_CUBE_MAP) {
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
  }

  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(type, GL_TEXTURE_MAX_LOD, maxLod);

  unbind();
}

Texture::~Texture() {
  if (isValid()) {
    glDeleteTextures(1, &id);
  }
}

void Texture::buffer2DRGBAData(const Image& image) {
  assert(type == GL_TEXTURE_2D);
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int32_t>(image.width), static_cast<int32_t>(image.height), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, &image.data[0]);
  glGenerateMipmap(type);
  unbind();
}

void Texture::bufferCubeMapRGBAData(const std::array<Ref<const Image>, 6>& images) {
  assert(type == GL_TEXTURE_CUBE_MAP);
  bind();

  for (size_t i = 0; i < images.size(); i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, static_cast<int32_t>(images[i]->width),
                 static_cast<int32_t>(images[i]->height), 0, GL_RGBA, GL_UNSIGNED_BYTE, &images[i]->data[0]);
  }
  glGenerateMipmap(type);

  unbind();
}

void Texture::bind() const {
  glBindTexture(type, id);
}

void Texture::bindToSlot(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  bind();
}

void Texture::unbind() const {
  glBindTexture(type, 0);
}

Ref<const Texture> Texture::loadTexture2D(const std::string& name) {
  Ref<const Image> image = AssetManager::instance().loadImage(name);
  if (image == nullptr) {
    return nullptr;
  }

  Ref<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_2D, 4);
  texture->buffer2DRGBAData(*image);
  return texture;
}

Ref<const Texture> Texture::loadCubeMapTexture(const std::string& name) {
  std::stringstream parts(name);

  std::array<Ref<const Image>, 6> images{};
  std::string imageName;
  AssetManager& assetManager = AssetManager::instance();
  for (int32_t i = 0; i < 6; i++) {
    if (!std::getline(parts, imageName, ';')) {
      std::cerr << "Invalid cube map name format" << std::endl;
      return nullptr;
    }
    images[i] = assetManager.loadImage(imageName);

    if (images[i] == nullptr) {
      return nullptr;
    }
  }

  Ref<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP);
  texture->bufferCubeMapRGBAData(images);
  return texture;
}
