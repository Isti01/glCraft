#pragma once
#include "type_traits"

class Buffer {
protected:
  uint32_t id = 0;
  Buffer() { glGenBuffers(1, &id); }

public:
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &) = delete;
  Buffer(Buffer &&) = delete;

  [[nodiscard]] uint32_t getId() const { return id; };
  [[nodiscard]] bool isValid() const { return id != 0; };

  ~Buffer() {
    if (isValid()) glDeleteBuffers(1, &id);
  };
};
