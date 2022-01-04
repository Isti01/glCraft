#include "Application.h"

Application::Application() : gui(Gui::instance()) {}

int Application::run() {
  if (!scene) return -1;
  if (!window.isValid()) return -1;

  scene->init();
  lastTick = Clock::now();
  while (!window.shouldClose()) {
    window.pollEvents();
    updateAndRender();
  }
  return 0;
}

void Application::updateAndRender() {
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

int32_t Application::getWindowWidth() {
  return window.getWindowWidth();
}

int32_t Application::getWindowHeight() {
  return window.getWindowHeight();
}

void Application::onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  scene->onKeyEvent(key, scancode, action, mode);
}

void Application::onMouseButtonEvent(int32_t button, int32_t action, int32_t mods) {
  scene->onMouseButtonEvent(button, action, mods);
}

void Application::onResized(int32_t width, int32_t height) {
  scene->onResized(width, height);
}

void Application::onRefreshWindow() {
  updateAndRender();
}

void Application::onCursorPositionEvent(double x, double y) {
  scene->onCursorPositionEvent(x, y);
}
