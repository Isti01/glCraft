#include "Image.h"

#include <span>

Image Image::subImage(glm::uvec2 offset, glm::uvec2 extent) const {
  assert((offset.x + extent.x) <= width && (offset.y + extent.y) <= height);

  Image NewImage{extent.x, extent.y};
  const uint32_t RowPitch = width * sizeof(uint32_t);
  for (uint32_t y = 0; y < extent.y; ++y) {
    const size_t SrcOffset = ((offset.y + y) * RowPitch) + (offset.x * sizeof(uint32_t));

    const auto SourceRow = std::span<const std::uint8_t>(data.data() + SrcOffset, extent.x * sizeof(uint32_t));

    NewImage.data.insert(NewImage.data.begin(), SourceRow.begin(), SourceRow.end());
  }

  return NewImage;
}
