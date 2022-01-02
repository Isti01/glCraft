#include "VertexArray.h"

void VertexArray::bind() {
  glBindVertexArray(id);
  if (vertexBuffer) vertexBuffer->bind();
  if (indexBuffer) indexBuffer->bind();
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::renderIndexed() {
  if (indexBuffer == nullptr) throw std::exception("Cannot draw un-indexed vertex stream");

  bind();
  glDrawElements(GL_TRIANGLES, indexBuffer->getSize(), indexBuffer->getType(), nullptr);
  unbind();
}

VertexArray::~VertexArray() {
  if (isValid()) { glDeleteVertexArrays(1, &id); }
}
void VertexArray::renderVertexStream() {
  if (!isValid()) return;
  if (indexBuffer != nullptr) throw std::exception("Cannot draw indexed vertex stream");

  bind();
  glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->getSize());
  unbind();
}
void VertexArray::addVertexAttributes(const std::vector<VertexAttribute> &vector, int32_t defaultVertexSize) {
  bind();
  for (size_t i = 0; i < vector.size(); i++) {
    const auto &[componentCount, type, shouldBeNormalized, vertexSize, offset] = vector[i];
    const auto normalize = shouldBeNormalized ? GL_TRUE : GL_FALSE;
    const auto stride = vertexSize ? vertexSize : defaultVertexSize;

    glVertexAttribPointer(i, componentCount, type, normalize, stride, reinterpret_cast<void *>(offset));
    glEnableVertexAttribArray(i);
  }
  unbind();
}
