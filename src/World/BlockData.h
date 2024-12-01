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

  glm::vec4 getColor() const {
    switch (type) {
      case BlockType::bedrock:
        return glm::vec4(0.341, 0.341, 0.341, 1);
      case BlockType::planks:
        return glm::vec4(0.706, 0.565, 0.353, 1);
      case BlockType::grass:
        return glm::vec4(0.376, 0.627, 0.212, 1);
      case BlockType::dirt:
        return glm::vec4(0.588, 0.424, 0.29, 1);
      case BlockType::sand:
        return glm::vec4(0.82, 0.792, 0.576, 1);
      case BlockType::stone:
        return glm::vec4(0.498, 0.498, 0.498, 1);
      case BlockType::cobblestone:
        return glm::vec4(0.427, 0.427, 0.427, 1);
      case BlockType::glass:
        return glm::vec4(0.996, 0.996, 0.996, 1);
      case BlockType::oak_wood:
        return glm::vec4(0.416, 0.333, 0.204, 1);
      case BlockType::oak_leaves:
        return glm::vec4(0.114, 0.506, 0.114, 1);
      case BlockType::water:
        return glm::vec4(0.216, 0.325, 0.655, 1);
      case BlockType::lava:
        return glm::vec4(0.988, 0.631, 0., 1);
      case BlockType::iron:
        return glm::vec4(0.914, 0.914, 0.914, 1);
      case BlockType::diamond:
        return glm::vec4(0.412, 0.875, 0.855, 1);
      case BlockType::gold:
        return glm::vec4(0.996, 0.984, 0.365, 1);
      case BlockType::obsidian:
        return glm::vec4(0.035, 0.035, 0.055, 1);
      case BlockType::sponge:
        return glm::vec4(0.898, 0.898, 0.306, 1);
      default:
        return glm::vec4(1, 1, 1, 1);
    }
  }

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
  BlockData(BlockData::BlockType type = BlockType::air) : type(type), blockClass(typeToClass(type)) {}
};
