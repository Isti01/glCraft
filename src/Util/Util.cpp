#include "Util.h"

Ref<std::string> Util::readBinaryFile(const std::string& path) {
  Ref<std::string> content = std::make_shared<std::string>();
  std::ifstream file(path, std::ios::in | std::ios::binary);

  if (!file) {
    std::cerr << "Failed to open the file: " << path << std::endl;
    return nullptr;
  }
  file.seekg(0, std::ios::end);
  auto length = file.tellg();

  if (length == -1) {
    std::cerr << "Failed to read the file: " << path << std::endl;
    return nullptr;
  }

  content->resize(length);
  file.seekg(0, std::ios::beg);
  file.read(&content->at(0), length);

  return content;
}

int32_t Util::positiveMod(int32_t num, int32_t divisor) {
  int32_t mod = num % divisor;

  if (mod < 0) {
    return mod + divisor;
  }
  return mod;
}
