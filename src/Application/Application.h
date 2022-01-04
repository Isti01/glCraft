#pragma once

#include "../Scene/Scene.h"
#include "../glCraft.h"
#include "Gui.h"
#include "Window.h"

class Application {
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using Clock = std::chrono::steady_clock;

  Window &window = Window::instance();
  Gui &gui;
  Ref<Scene> scene;
  TimePoint lastTick = Clock::now();

  Application();

  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
  void onCursorPositionEvent(double x, double y);
  void onResized(int32_t width, int32_t height);

  void onRefreshWindow();
  void updateAndRender();

  friend Window;

public:
  Application(const Application &) = delete;
  Application(Application &) = delete;
  Application(Application &&) = delete;

  static Application &instance() {
    static Application app;
    return app;
  };

  void setScene(Ref<Scene> &newScene) { scene = newScene; };

  int32_t getWindowWidth();
  int32_t getWindowHeight();
  Window &getWindow() { return window; };
  int run();
};
