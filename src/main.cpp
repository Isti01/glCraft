#include "Application/Application.h"
#include "Performance/Trace.h"
#include "glCraft.h"

int main(int argc, char **argv) {
  START_TRACE("startup.json");
  auto *app = new Application;
  END_TRACE();

  START_TRACE("scene-creation.json");
  std::string savePath = argc > 1 ? argv[1] : "default.glc";
  app->setScene(std::make_shared<Scene>(savePath));
  END_TRACE();

  START_TRACE("runtime.json");
  int result;
  result = app->run();
  END_TRACE();

  START_TRACE("shutdown.json");
  delete app;
  END_TRACE();

  return result;
}
