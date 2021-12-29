#pragma once

#include "../Scene/Scene.h"
#include "../glCraft.h"
#include "Gui.h"
#include "Window.h"

class Application {
private:
  Window &window = Window::instance();
  Gui &gui;
  Scene *scene = nullptr;
  friend Window;

  Application();

  void onKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
  void onMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
  void onResized(int32_t width, int32_t height);

public:
  Application(const Application &) = delete;
  Application(Application &) = delete;
  Application(Application &&) = delete;

  static Application &instance() {
    static Application app;
    return app;
  };

  void setScene(Scene &newScene) { scene = &newScene; };

  Window &getWindow() { return window; };
  int run();
};
