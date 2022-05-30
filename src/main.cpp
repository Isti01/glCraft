#include "Application/Application.h"
#include "glCraft.h"

int main(int argc, char **argv) {
  std::string savePath = argc > 1 ? argv[1] : "default.glc";

  Application app;
  app.setScene(std::make_shared<Scene>(savePath));
  return app.run();
}
