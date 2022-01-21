#pragma once

#include "../glCraft.h"

class Util {
  Util() = default;

public:
  static Ref<std::string> readBinaryFile(const std::string& path);
  static int32_t positiveMod(int32_t num, int32_t divisor);
};