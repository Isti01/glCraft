#pragma once

#include "../Rendering/Image.h"
#include "AssetRegistry.h"

class ImageRegistry : public AssetRegistry<Image> {
  Ref<const Image> loadAsset(const std::string& name) override {
    Ref<Image> image = std::make_shared<Image>();
    uint32_t error = lodepng::decode(image->data, image->width, image->height, name, LCT_RGBA);

    if (error != 0) {
      std::cerr << "Failed to read the image file: " << name << ", message: " << lodepng_error_text(error) << std::endl;
      return nullptr;
    }

    return image;
  }
};