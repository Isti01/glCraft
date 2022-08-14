#include "VertexArray.h"

#include "../Performance/Trace.h"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &id);
  bind();

  vertexBuffer = VertexBuffer::createRef();
  vertexBuffer->bind();

  unbind();
}

VertexArray::~VertexArray() {
  if (isValid()) {
    glDeleteVertexArrays(1, &id);
  }
}

void VertexArray::bind() {
  glBindVertexArray(id);
  if (vertexBuffer)
    vertexBuffer->bind();
  if (indexBuffer)
    indexBuffer->bind();
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::renderIndexed(int32_t type) {
  TRACE_FUNCTION();
  assert(indexBuffer != nullptr);

  bind();
  glDrawElements(type, indexBuffer->getSize(), indexBuffer->getType(), nullptr);
  unbind();
}

void VertexArray::renderVertexStream(int32_t type) {
  TRACE_FUNCTION();
  renderVertexSubStream(vertexBuffer->getSize(), 0, type);
}

void VertexArray::renderVertexSubStream(int32_t size, int32_t startOffset, int32_t type) {
  TRACE_FUNCTION();
  if (!isValid())
    return;
  assert(indexBuffer == nullptr);

  bind();
  glDrawArrays(type, startOffset, size);
  unbind();
}

void VertexArray::addVertexAttributes(const std::vector<VertexAttribute> &vector, int32_t defaultVertexSize) {
  bind();
  for (size_t i = 0; i < vector.size(); i++) {
    const auto &[componentCount, type, shouldBeNormalized, vertexSize, offset] = vector[i];
    const auto normalize = shouldBeNormalized ? GL_TRUE : GL_FALSE;
    const auto stride = vertexSize ? vertexSize : defaultVertexSize;

    switch (type) {
      case VertexAttribute::UShort:
      case VertexAttribute::Int:
      case VertexAttribute::UInt:
      case VertexAttribute::Byte:
        glVertexAttribIPointer(i, componentCount, type, stride, reinterpret_cast<void *>(offset));
        break;
      case VertexAttribute::Float:
        glVertexAttribPointer(i, componentCount, type, normalize, stride, reinterpret_cast<void *>(offset));
        break;
    }
    glEnableVertexAttribArray(i);
  }
  unbind();
}
