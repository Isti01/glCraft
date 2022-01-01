#include "Application.h"

Application::Application() : gui(Gui::instance()) {}

int Application::run() {
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using Clock = std::chrono::steady_clock;
  if (!scene) return -1;
  if (!window.isValid()) return -1;

  scene->init();

  TimePoint lastTick = Clock::now();
  while (!window.shouldClose()) {
    TimePoint now = Clock::now();
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

int32_t Application::getWindowWidth() {
  return window.getWindowWidth();
}

int32_t Application::getWindowHeight() {
  return window.getWindowHeight();
}

void Application::onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  std::cout << "onKeyEvent" << std::endl;
}

void Application::onMouseButtonEvent(int32_t button, int32_t action, int32_t mods) {
  std::cout << "onMouseButtonEvent" << std::endl;
}

void Application::onResized(int32_t width, int32_t height) {
  scene->onResized(width, height);
  std::cout << "onResized" << std::endl;
}
