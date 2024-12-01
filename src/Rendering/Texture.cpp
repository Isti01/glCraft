#include "Texture.h"

#include "../AssetManager/AssetManager.h"
#include "../Performance/Trace.h"
#include "Image.h"

Texture::Texture(uint32_t target,
                 int32_t internalFormat,
                 int32_t format,
                 int32_t type,
                 bool generateMipMap,
                 int32_t maxLod)
    : target(target),
      internalFormat(internalFormat),
      format(format),
      type(type),
      generateMipMap(generateMipMap) {
  assert(target == GL_TEXTURE_2D || target == GL_TEXTURE_2D_ARRAY || target == GL_TEXTURE_CUBE_MAP);
  glGenTextures(1, &id);
  bind();


  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, generateMipMap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (generateMipMap) {
    glTexParameteri(target, GL_TEXTURE_MAX_LOD, maxLod);
  }

  unbind();
}

Texture::~Texture() {
  if (isValid()) {
    glDeleteTextures(1, &id);
  }
}


void Texture::allocateTexture(int32_t width, int32_t height) {
  TRACE_FUNCTION();
  assert(target == GL_TEXTURE_2D);
  bind();

  glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, nullptr);
  if (generateMipMap) {
    glGenerateMipmap(target);
  }

  unbind();
}

void Texture::buffer2DRGBAData(const Image& image) {
  TRACE_FUNCTION();
  assert(target == GL_TEXTURE_2D);
  bind();
  glTexImage2D(target, 0, internalFormat, static_cast<int32_t>(image.width), static_cast<int32_t>(image.height),
               0, format, type, &image.data[0]);
  if (generateMipMap) {
    glGenerateMipmap(target);
  }
  unbind();
}

void Texture::buffer2DArrayRGBAData(std::span<const Image> images) {
  TRACE_FUNCTION();
  assert(target == GL_TEXTURE_2D_ARRAY);
  assert(!images.empty());
  bind();

  glTexImage3D(target, 0, internalFormat, static_cast<int32_t>(images[0].width),
               static_cast<int32_t>(images[0].height), images.size(), 0, format, type, nullptr);

  for (size_t i = 0; i < images.size(); i++) {
    glTexSubImage3D(target, 0, 0, 0, i, static_cast<int32_t>(images[i].width),
                    static_cast<int32_t>(images[i].height), 1, format, type, images[i].data.data());
  }
  if (generateMipMap) {
    glGenerateMipmap(target);
  }

  unbind();
}

void Texture::bufferCubeMapRGBAData(std::span<Ref<const Image>, 6> images) {
  TRACE_FUNCTION();
  assert(target == GL_TEXTURE_CUBE_MAP);
  bind();

  for (size_t i = 0; i < images.size(); i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, static_cast<int32_t>(images[i]->width),
                 static_cast<int32_t>(images[i]->height), 0, format, type, &images[i]->data[0]);
  }
  if (generateMipMap) {
    glGenerateMipmap(target);
  }

  unbind();
}

void Texture::bind() const {
  glBindTexture(target, id);
}

void Texture::bindToSlot(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  bind();
}

void Texture::unbind() const {
  glBindTexture(target, 0);
}

Ref<const Texture> Texture::loadTexture2D(const std::string& name) {
  TRACE_FUNCTION();
  Ref<const Image> image = AssetManager::instance().loadImage(name);
  if (image == nullptr) {
    return nullptr;
  }

  Ref<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4);
  texture->buffer2DRGBAData(*image);
  return texture;
}

Ref<const Texture> Texture::loadTexture2DArray(const std::string& name) {
  TRACE_FUNCTION();
  Ref<const Image> image = AssetManager::instance().loadImage(name);
  if (image == nullptr) {
    return nullptr;
  }

  // Split up image
  // Assume 16x16 splits
  const uint8_t tileWidth = 16;
  const uint8_t tileHeight = 16;
  std::vector<Image> subImages;
  for (uint32_t tileY = 0; tileY < image->height / tileHeight; ++tileY) {
    for (uint32_t tileX = 0; tileX < image->width / tileWidth; ++tileX) {
      subImages.emplace_back(image->subImage({tileX * tileWidth, tileY * tileHeight}, {tileWidth, tileHeight}));
    }
  }

  Ref<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_2D_ARRAY, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4);
  texture->buffer2DArrayRGBAData(subImages);
  return texture;
}
Ref<const Texture> Texture::loadCubeMapTexture(const std::string& name) {
  TRACE_FUNCTION();
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

  Ref<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
  texture->bufferCubeMapRGBAData(images);
  return texture;
}
