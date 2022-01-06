#pragma once

struct BlockData {
  enum class BlockType {
    grass,
    dirt,
    sand,
    stone,
    cobble_stone,
    glass,
    oak_wood,
    oak_leaves,
    air
  };
  BlockType type = BlockType::air;

  static bool isTransparent(BlockType type) { return type == BlockType::glass; }
  BlockData(BlockData::BlockType type = BlockType::air) : type(type){};
};
