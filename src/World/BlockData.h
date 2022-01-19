#pragma once

struct BlockData {
  enum class BlockClass {
    air,
    solid,
    semiTransparent,
    transparent
  };

  enum class BlockType {
    bedrock,
    planks,
    grass,
    dirt,
    sand,
    stone,
    cobblestone,
    glass,
    oak_wood,
    oak_leaves,
    water,
    lava,
    iron,
    diamond,
    gold,
    obsidian,
    sponge,
    air
  };

  BlockType type;
  BlockClass blockClass;

  static BlockClass typeToClass(BlockType type) {
    if (type == BlockType::air) {
      return BlockClass::air;
    } else if (type == BlockType::water) {
      return BlockClass::semiTransparent;
    } else if (type == BlockType::oak_leaves || type == BlockType::glass) {
      return BlockClass::transparent;
    }

    return BlockClass::solid;
  }
  BlockData(BlockData::BlockType type = BlockType::air) : type(type), blockClass(typeToClass(type)){};
};
