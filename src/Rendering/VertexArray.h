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
  class VertexBuffer : public Buffer {
  public:
    static Ref<VertexBuffer> createRef() { return std::make_shared<VertexBuffer>(); }

    void bind() { glBindBuffer(GL_ARRAY_BUFFER, id); };

    template<typename T>
    void bufferStaticVertexData(const std::vector<T> &data, const std::vector<VertexAttribute> &vertexAttributes) {
      if (!isValid()) throw std::exception("Cannot write data to an invalid buffer");

      bind();
      glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), &data[0], GL_STATIC_DRAW);

      for (size_t i = 0; i < vertexAttributes.size(); i++) {
        const auto &[componentCount, type, shouldBeNormalized, vertexSize, offset] = vertexAttributes[i];
        const auto normalize = shouldBeNormalized ? GL_TRUE : GL_FALSE;
        const auto stride = vertexSize ? vertexSize : sizeof(T);

        glVertexAttribPointer(i, componentCount, type, normalize, stride, reinterpret_cast<void *>(offset));
        glEnableVertexAttribArray(i);
      }
    }
  };

  class IndexBuffer : public Buffer {
    int elementCount = 0;
    uint32_t type = 0;

  public:
    static Ref<IndexBuffer> createRef() { return std::make_shared<IndexBuffer>(); }

    void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); };

    [[nodiscard]] uint32_t getType() const { return type; }
    [[nodiscard]] int getElementCount() const { return elementCount; };

    template<typename T>
    void bufferStaticIndexData(const std::vector<T> &data) {
      static_assert(std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value ||
                       std::is_same<T, unsigned int>::value,
                    "The given type must be either unsigned char, unsigned short or unsigned int");

      if (!isValid()) throw std::exception("Cannot write data to an invalid buffer");

      elementCount = data.size();

      switch (sizeof(T)) {
        case 1:
          type = GL_UNSIGNED_BYTE;
          break;
        case 2:
          type = GL_UNSIGNED_SHORT;
          break;
        case 4:
          type = GL_UNSIGNED_INT;
          break;
      }

      bind();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * data.size(), &data[0], GL_STATIC_DRAW);
    }
  };

  uint32_t id = 0;
  Ref<VertexBuffer> vertexBuffer;
  Ref<IndexBuffer> indexBuffer;

public:
  template<typename VertexT, typename IndexT>
  VertexArray(const std::vector<VertexT> &vertices,
              const std::vector<VertexAttribute> &vertexAttributes,
              const std::vector<IndexT> &indices) {
    glGenVertexArrays(1, &id);
    bind();

    vertexBuffer = VertexBuffer::createRef();
    vertexBuffer->bufferStaticVertexData<VertexT>(vertices, vertexAttributes);

    indexBuffer = IndexBuffer::createRef();
    indexBuffer->bufferStaticIndexData<IndexT>(indices);

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
