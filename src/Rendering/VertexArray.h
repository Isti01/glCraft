#pragma once

#include "Buffer.h"

struct VertexAttribute {
  enum ComponentType {
    UShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UInt = GL_UNSIGNED_INT,
    Float = GL_FLOAT,
  };

  uint8_t componentCount;
  ComponentType type;
  bool shouldBeNormalized = false;
  uint32_t vertexSize;
  uint32_t offset;

  VertexAttribute(uint8_t componentCount, ComponentType type, uint32_t offset)
      : componentCount(componentCount),
        type(type),
        shouldBeNormalized(false),
        vertexSize(0),
        offset(offset) {}
};

class VertexArray {
  uint32_t id = 0;
  Ref<VertexBuffer> vertexBuffer;
  Ref<IndexBuffer> indexBuffer;

public:
  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices, const std::vector<IndexT> &indices) {
    glGenVertexArrays(1, &id);
    bind();

    vertexBuffer = VertexBuffer::createRef();
    vertexBuffer->bufferStaticVertexData<VertexT>(vertices);

    indexBuffer = IndexBuffer::createRef();
    indexBuffer->bufferStaticIndexData<IndexT>(indices);

    unbind();
  };

  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices,
              const std::vector<VertexAttribute> &vertexAttributes,
              const std::vector<IndexT> &indices)
      : VertexArray(vertices, indices) {
    bind();
    for (size_t i = 0; i < vertexAttributes.size(); i++) {
      const auto &[componentCount, type, shouldBeNormalized, vertexSize, offset] = vertexAttributes[i];
      const auto normalize = shouldBeNormalized ? GL_TRUE : GL_FALSE;
      const auto stride = vertexSize ? vertexSize : sizeof(VertexT);

      glVertexAttribPointer(i, componentCount, type, normalize, stride, reinterpret_cast<void *>(offset));
      glEnableVertexAttribArray(i);
    }

    unbind();
  };

  VertexArray(const VertexArray &) = delete;
  VertexArray(VertexArray &) = delete;
  VertexArray(VertexArray &&) = delete;

  void bind();
  void render();
  void unbind();

  [[nodiscard]] bool isValid() const { return id != 0; };

  ~VertexArray();
};
