#include "Application.h"
#include <iostream>

int main() {
  std::cout << "Hello World from glCraft!" << std::endl;

  return Application::instance().run();
}
