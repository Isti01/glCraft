#include "VertexArray.h"

void VertexArray::bind() {
  glBindVertexArray(id);
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::render()  {
  bind();
  glDrawElements(GL_TRIANGLES, indexBuffer->getElementCount(), indexBuffer->getType(), 0);
}

VertexArray::~VertexArray() {
  if (isValid()) { glDeleteVertexArrays(1, &id); }
}
