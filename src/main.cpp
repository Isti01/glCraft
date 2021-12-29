#include <iostream>

#include "Application/Application.h"

int main() {
  std::cout << "Hello World from glCraft!" << std::endl;

  Application &app = Application::instance();
  Scene scene = Scene();
  app.setScene(scene);

  return app.run();
}
