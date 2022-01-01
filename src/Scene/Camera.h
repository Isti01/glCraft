#include "../glCraft.h"

class Camera {
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 up;

  glm::mat4 view;
  glm::mat4 calcView() { return glm::lookAt(position, direction, up); }

public:
  Camera();

  const glm::mat4& setPosition(glm::vec3 eye);
  const glm::mat4& lookAt(glm::vec3 eye, glm::vec3 center);

  [[nodiscard]] const glm::mat4& getViewMatrix() const { return view; }
};
