#pragma once

#include "AssetRegistry.h"

class TextRegistry : public AssetRegistry<std::string> {
  Ref<const std::string> loadAsset(const std::string &name) override {
    std::string content;
    std::ifstream file(name, std::ios::in | std::ios::binary);

    if (!file) {
      std::cerr << "Failed to open the file: " << name << std::endl;
      return nullptr;
    }
    file.seekg(0, std::ios::end);
    auto length = file.tellg();

    if (length == -1) {
      std::cerr << "Failed to read the file: " << name << std::endl;
      return nullptr;
    }

    content.resize(length);
    file.seekg(0, std::ios::beg);
    file.read(&content[0], length);

    return std::make_shared<std::string>(content);
  }
};
