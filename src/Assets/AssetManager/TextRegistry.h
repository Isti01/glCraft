#pragma once
#include "AssetRegistry.h"

class TextRegistry : public AssetRegistry<std::string> {
  Ref<const std::string> loadAsset(const std::string &name) override {
    try {
      std::ifstream file;

      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      file.open("assets/" + name);

      std::stringstream ss;
      ss << file.rdbuf();

      return std::make_shared<std::string>(ss.str());
    } catch (const std::exception &ex) {
      std::cout << ex.what() << std::endl;
      return nullptr;
    }
  }
};
