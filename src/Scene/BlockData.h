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
  BlockType type;

  BlockData(BlockData::BlockType type = BlockType::air) : type(type){};
};
