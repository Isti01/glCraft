#include "Application/Application.h"
#include "glCraft.h"

int main() {
  std::cout << "Hello World from glCraft!" << std::endl;

  Application &app = Application::instance();
  Ref<Scene> scene = std::make_shared<Scene>();
  app.setScene(scene);

  return app.run();
}
