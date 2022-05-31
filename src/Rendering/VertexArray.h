#pragma once

#include <vector>

#include "../glCraft.h"
#include "Buffer.h"

struct VertexAttribute {
  enum ComponentType {
    UShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UInt = GL_UNSIGNED_INT,
    Byte = GL_BYTE,
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
  explicit VertexArray();
  ~VertexArray();

  template<typename VertexT>
  explicit VertexArray(const std::vector<VertexT> &vertices, bool dynamic = false) {
    glGenVertexArrays(1, &id);
    bind();

    vertexBuffer = VertexBuffer::createRef();
    if (dynamic) {
      vertexBuffer->bufferDynamicData<VertexT>(vertices);
    } else {
      vertexBuffer->bufferStaticData<VertexT>(vertices);
    }

    unbind();
  };


  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices, const std::vector<IndexT> &indices, bool dynamic = false)
      : VertexArray(vertices, dynamic) {
    bind();
    indexBuffer = IndexBuffer::createRef<IndexT>();

    if (dynamic) {
      indexBuffer->bufferDynamicData<IndexT>(indices);
    } else {
      indexBuffer->bufferStaticData<IndexT>(indices);
    }
    unbind();
  };

  template<typename VertexT>
  explicit VertexArray(const std::vector<VertexT> &vertices,
                       const std::vector<VertexAttribute> &vertexAttributes,
                       bool dynamic = false)
      : VertexArray(vertices, dynamic) {
    addVertexAttributes(vertexAttributes, sizeof(VertexT));
  };

  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices,
              const std::vector<VertexAttribute> &vertexAttributes,
              const std::vector<IndexT> &indices,
              bool dynamic = false)
      : VertexArray(vertices, indices, dynamic) {
    addVertexAttributes(vertexAttributes, sizeof(VertexT));
  };


  void bind();
  void addVertexAttributes(const std::vector<VertexAttribute> &vector, int32_t defaultVertexSize);
  void renderIndexed(int32_t type = GL_TRIANGLES);
  void renderVertexSubStream(int32_t size, int32_t startOffset, int32_t type = GL_TRIANGLES);
  void renderVertexStream(int32_t type = GL_TRIANGLES);
  void unbind();

  Ref<VertexBuffer> getVertexBuffer() { return vertexBuffer; };

  [[nodiscard]] bool isValid() const { return id != 0; };

  VertexArray(const VertexArray &) = delete;
  VertexArray(VertexArray &) = delete;
  VertexArray(VertexArray &&) noexcept = delete;
  VertexArray &operator=(VertexArray &) = delete;
  VertexArray &operator=(VertexArray &&) noexcept = delete;
};
