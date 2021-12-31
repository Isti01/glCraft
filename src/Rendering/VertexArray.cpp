#include "VertexArray.h"

void VertexArray::bind() {
  glBindVertexArray(id);
  if (vertexBuffer) vertexBuffer->bind();
  if (indexBuffer) indexBuffer->bind();
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::render() {
  bind();
  glDrawElements(GL_TRIANGLES, indexBuffer->getElementCount(), indexBuffer->getType(), nullptr);
  unbind();
}

VertexArray::~VertexArray() {
  if (isValid()) { glDeleteVertexArrays(1, &id); }
}
