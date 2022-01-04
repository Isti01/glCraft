#pragma once

#include "type_traits"

class Buffer {
protected:
  int32_t size;
  uint32_t id = 0;
  Buffer() { glGenBuffers(1, &id); }

public:
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &) = delete;
  Buffer(Buffer &&) = delete;

  [[nodiscard]] int32_t getSize() const { return size; }
  [[nodiscard]] uint32_t getId() const { return id; };
  [[nodiscard]] bool isValid() const { return id != 0; };

  ~Buffer() {
    if (isValid()) glDeleteBuffers(1, &id);
  };
};

class VertexBuffer : public Buffer {
public:
  static Ref<VertexBuffer> createRef() { return std::make_shared<VertexBuffer>(); }

  void bind() { glBindBuffer(GL_ARRAY_BUFFER, id); };

  template<typename T>
  void bufferStaticVertexData(const std::vector<T> &data) {
    if (!isValid()) throw std::exception("Cannot write data to an invalid buffer");

    bind();
    size = data.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, &data[0], GL_STATIC_DRAW);
  }
};

class IndexBuffer : public Buffer {
  uint32_t type = 0;

public:
  static Ref<IndexBuffer> createRef() { return std::make_shared<IndexBuffer>(); }

  void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); };

  [[nodiscard]] uint32_t getType() const { return type; }

  template<typename T>
  void bufferStaticIndexData(const std::vector<T> &data) {
    static_assert(std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value ||
                     std::is_same<T, unsigned int>::value,
                  "The given type must be either unsigned char, unsigned short or unsigned int");

    if (!isValid()) throw std::exception("Cannot write data to an invalid buffer");


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
    size = data.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * size, &data[0], GL_STATIC_DRAW);
  }
};