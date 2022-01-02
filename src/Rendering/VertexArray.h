#pragma once

#include <vector>

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
  int32_t vertexSize;
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
  template<typename VertexT>
  explicit VertexArray(const std::vector<VertexT> &vertices) {
    glGenVertexArrays(1, &id);
    bind();

    vertexBuffer = VertexBuffer::createRef();
    vertexBuffer->bufferStaticVertexData<VertexT>(vertices);

    unbind();
  };


  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices, const std::vector<IndexT> &indices) : VertexArray(vertices) {
    bind();

    indexBuffer = IndexBuffer::createRef();
    indexBuffer->bufferStaticIndexData<IndexT>(indices);

    unbind();
  };

  template<typename VertexT>
  explicit VertexArray(const std::vector<VertexT> &vertices, const std::vector<VertexAttribute> &vertexAttributes)
      : VertexArray(vertices) {
    addVertexAttributes(vertexAttributes, sizeof(VertexT));
  };

  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices,
              const std::vector<VertexAttribute> &vertexAttributes,
              const std::vector<IndexT> &indices)
      : VertexArray(vertices, indices) {
    addVertexAttributes(vertexAttributes, sizeof(VertexT));
  };

  VertexArray(const VertexArray &) = delete;
  VertexArray(VertexArray &) = delete;
  VertexArray(VertexArray &&) = delete;

  void bind();
  void addVertexAttributes(const std::vector<VertexAttribute> &vector, int32_t defaultVertexSize);
  void renderIndexed();
  void renderVertexStream();
  void unbind();

  [[nodiscard]] bool isValid() const { return id != 0; };

  ~VertexArray();
};
