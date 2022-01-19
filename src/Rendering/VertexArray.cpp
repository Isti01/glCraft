#include "VertexArray.h"

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
  assert(indexBuffer != nullptr);

  bind();
  glDrawElements(type, indexBuffer->getSize(), indexBuffer->getType(), nullptr);
  unbind();
}

VertexArray::~VertexArray() {
  if (isValid()) {
    glDeleteVertexArrays(1, &id);
  }
}

void VertexArray::renderVertexStream(int32_t type) {
  renderVertexSubStream(vertexBuffer->getSize(), 0, type);
}

void VertexArray::renderVertexSubStream(int32_t size, int32_t startOffset, int32_t type) {
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
