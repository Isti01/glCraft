#pragma once

#include "../glCraft.h"
#include "type_traits"

class Buffer {
protected:
  int32_t size = 0;
  uint32_t id = 0;
  Buffer() { glGenBuffers(1, &id); }

public:
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &) = delete;
  Buffer(Buffer &&) = delete;

  [[nodiscard]] int32_t getSize() const { return size; }
  [[maybe_unused]] [[nodiscard]] uint32_t getId() const { return id; };
  [[nodiscard]] bool isValid() const { return id != 0; };

  ~Buffer() {
    if (isValid())
      glDeleteBuffers(1, &id);
  };
};

class VertexBuffer : public Buffer {
public:
  static Ref<VertexBuffer> createRef() { return std::make_shared<VertexBuffer>(); }

  void bind() { glBindBuffer(GL_ARRAY_BUFFER, id); };

  template<typename T>
  void bufferStaticVertexData(const std::vector<T> &data) {
    assert(isValid() && "Cannot write data to an invalid buffer");

    bind();
    size = data.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, &data[0], GL_STATIC_DRAW);
  }

  template<typename T>
  void bufferDynamicVertexData(const std::vector<T> &data) {
    assert(isValid() && "Cannot write data to an invalid buffer");

    bind();
    size = data.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, &data[0], GL_DYNAMIC_DRAW);
  }

  template<typename T>
  void bufferDynamicVertexSubData(const std::vector<T> &data, int32_t offset = 0) {
    assert(isValid() && "Cannot write data to an invalid buffer");

    bind();
    size = data.size();
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(T) * size, &data[0]);
  }
};

class IndexBuffer : public Buffer {
  uint32_t type = 0;

public:
  static Ref<IndexBuffer> createRef() { return std::make_shared<IndexBuffer>(); }

  void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); };

  [[nodiscard]] uint32_t getType() const { return type; }

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
  }

  template<typename T>
  void bufferStaticIndexData(const std::vector<T> &data) {
    static_assert(std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value ||
                     std::is_same<T, unsigned int>::value,
                  "The given type must be either unsigned char, unsigned short or unsigned int");

    assert(isValid() && "Cannot write data to an invalid buffer");
    type = getSizeType<T>();

    bind();
    size = data.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * size, &data[0], GL_STATIC_DRAW);
  }

  template<typename T>
  void bufferDynamicIndexData(const std::vector<T> &data) {
    static_assert(std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value ||
                     std::is_same<T, unsigned int>::value,
                  "The given type must be either unsigned char, unsigned short or unsigned int");

    assert(isValid() && "Cannot write data to an invalid buffer");
    type = getSizeType<T>();

    bind();
    size = data.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * size, &data[0], GL_DYNAMIC_DRAW);
  }

  template<typename T>
  [[maybe_unused]] void bufferDynamicIndexSubData(const std::vector<T> &data, int32_t offset = 0) {
    static_assert(std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value ||
                     std::is_same<T, unsigned int>::value,
                  "The given type must be either unsigned char, unsigned short or unsigned int");

    assert(isValid() && "Cannot write data to an invalid buffer");
    type = getSizeType<T>();

    bind();
    size = data.size();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(T) * size, &data[0]);
  }
};