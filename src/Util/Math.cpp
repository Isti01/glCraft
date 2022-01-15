#include "Math.h"

int32_t Math::positiveMod(int32_t num, int32_t divisor) {
  int32_t mod = num % divisor;

  if (mod < 0) {
    return mod + divisor;
  }
  return mod;
}