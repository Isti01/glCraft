#pragma once

#include "../Performance/Trace.h"
#include "../glCraft.h"
#include "type_traits"

class Buffer {
protected:
  int32_t size = 0;
  uint32_t id = 0;
  uint32_t type;


  Buffer(uint32_t type) : type(type) { glGenBuffers(1, &id); }

public:
  ~Buffer() {
    if (isValid())
      glDeleteBuffers(1, &id);
  };

  void bind() { glBindBuffer(type, id); }

  template<typename T>
  void bufferStaticData(const std::vector<T> &data, int32_t dataSize, int32_t dataOffset = 0) {
    TRACE_FUNCTION();
    assert(isValid() && "Cannot write data to an invalid buffer");
    assert(dataOffset + dataSize <= data.size() && "Data is out of bounds");

    bind();
    size = dataSize;
    glBufferData(type, sizeof(T) * size, &data[dataOffset], GL_STATIC_DRAW);
  }

  template<typename T>
  void bufferStaticData(const std::vector<T> &data) {
    bufferStaticData(data, data.size());
  }

  template<typename T>
  void bufferDynamicData(const std::vector<T> &data, int32_t dataSize, int32_t dataOffset = 0) {
    TRACE_FUNCTION();
    assert(isValid() && "Cannot write data to an invalid buffer");
    assert(dataOffset + dataSize <= data.size() && "Data is out of bounds");

    bind();
    size = dataSize;
    glBufferData(type, sizeof(T) * size, &data[dataOffset], GL_DYNAMIC_DRAW);
  }

  template<typename T>
  void bufferDynamicData(const std::vector<T> &data) {
    bufferDynamicData(data, data.size());
  }

  template<typename T>
  void bufferDynamicSubData(const std::vector<T> &data,
                            int32_t dataSize,
                            int32_t dataOffset = 0,
                            int32_t bufferOffset = 0) {
    TRACE_FUNCTION();
    assert(isValid() && "Cannot write data to an invalid buffer");
    assert(dataOffset + dataSize <= data.size() && "Data is out of bounds");
    assert(dataOffset + dataSize <= size && "Buffer is out of bounds");

    bind();
    size = dataSize;
    glBufferSubData(type, bufferOffset * sizeof(T), sizeof(T) * size, &data[dataOffset]);
  }

  [[nodiscard]] int32_t getSize() const { return size; }
  [[maybe_unused]] [[nodiscard]] uint32_t getId() const { return id; };
  [[nodiscard]] bool isValid() const { return id != 0; };


  Buffer(const Buffer &) = delete;
  Buffer(Buffer &) = delete;
  Buffer(Buffer &&) noexcept = delete;
  Buffer &operator=(Buffer &) = delete;
  Buffer &operator=(Buffer &&) noexcept = delete;
};

class VertexBuffer : public Buffer {
public:
  VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}
  static Ref<VertexBuffer> createRef() { return std::make_shared<VertexBuffer>(); }
};


class IndexBuffer : public Buffer {
  uint32_t type = 0;

  template<typename T>
  int32_t getSizeType() {
    switch (sizeof(T)) {
      case 1:
        return GL_UNSIGNED_BYTE;
      case 2:
        return GL_UNSIGNED_SHORT;
      case 4:
        return GL_UNSIGNED_INT;
    }

    assert(false);
    return 0;
  }

public:
  template<typename T>
  explicit IndexBuffer(T _) : Buffer(GL_ELEMENT_ARRAY_BUFFER) {
    static_assert(
       std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value || std::is_same<T, uint32_t>::value,
       "The given type must be either unsigned char, unsigned short or unsigned int");
    type = getSizeType<T>();
  }

  template<typename T>
  static Ref<IndexBuffer> createRef() {
    return std::make_shared<IndexBuffer>(T());
  }

  [[nodiscard]] uint32_t getType() const { return type; }
};