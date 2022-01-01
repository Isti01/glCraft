#include "Camera.h"
Camera::Camera() : position(), direction(), up({0, 1, 0}), view(calcView()){};

const glm::mat4& Camera::lookAt(glm::vec3 eye, glm::vec3 center) {
  position = eye;
  direction = center;
  return view = calcView();
}
const glm::mat4& Camera::setPosition(glm::vec3 eye) {
  position = eye;
  return view = calcView();
}
