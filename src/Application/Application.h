#pragma once

#include "../glCraft.h"
#include "Window.h"

class Application {
private:
  Window window = Window::instance();

  Application() = default;

public:
  static Application &instance() {
    static Application app;
    return app;
  };

  Application(const Application &) = delete;
  Application(Application &) = delete;
  Application(Application &&) = delete;
  Window &getWindow();

  int run();

  virtual ~Application();
};
