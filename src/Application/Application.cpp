#include "Application.h"

Application::Application() : gui(Gui::instance()) {}

int Application::run() {
  if (!scene) return -1;
  if (!window.isValid()) return -1;

  scene->init();
  while (!window.shouldClose()) {
    window.update();
    gui.update();

    scene->update();
    scene->render();
    scene->renderGui();

    gui.render();
    window.swapBuffers();
  }
  return 0;
}

void Application::onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  std::cout << "onKeyEvent" << std::endl;
}

void Application::onMouseButtonEvent(int32_t button, int32_t action, int32_t mods) {
  std::cout << "onMouseButtonEvent" << std::endl;
}

void Application::onResized(int32_t width, int32_t height) {
  std::cout << "onResized" << std::endl;
}
