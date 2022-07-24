#pragma once

#include "../glCraft.h"

class Gui {
private:
  static Gui *instancePtr;

public:
  Gui();
  ~Gui();

  static Gui &instance() { return *instancePtr; }

  void beginFrame();
  void finalizeFrame();

  Gui(const Gui &) = delete;
  Gui(Gui &&) noexcept = delete;
  Gui(Gui &) = delete;
  Gui &operator=(Gui &) = delete;
  Gui &operator=(Gui &&) noexcept = delete;
};
