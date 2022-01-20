#include "Application/Application.h"
#include "glCraft.h"

int main() {
  Application &app = Application::instance();
  app.setScene(std::make_shared<Scene>());
  return app.run();
}
