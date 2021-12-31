#include "Application.h"

Application::Application() : gui(Gui::instance()) {}

int Application::run() {
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  if (!scene) return -1;
  if (!window.isValid()) return -1;

  scene->init();

  TimePoint lastTick = std::chrono::steady_clock::now();
  while (!window.shouldClose()) {
    TimePoint now = std::chrono::steady_clock::now();
    float deltaTime = static_cast<float>((now - lastTick).count()) / 1000000000.0f;
    lastTick = now;

    window.update();
    gui.update();

    scene->update(deltaTime);
    scene->render();
    scene->renderGui();

    gui.finalizeFrame();
    window.finalizeFrame();
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
