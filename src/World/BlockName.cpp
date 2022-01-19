#include "BlockName.h"

const char* BlockName::blockTypeToName(BlockData::BlockType type) {
  int32_t index = blockTypeToIndex(type);
  if (index >= 0) {
    return BlockNames[index].second;
  }

  return "unknown";
}
int32_t BlockName::blockTypeToIndex(BlockData::BlockType type) {
  for (int32_t i = 0; i < BlockNames.size(); ++i) {
    if (type == BlockNames[i].first) {
      return i;
    }
  }

  return -1;
}
