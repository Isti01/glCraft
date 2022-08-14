#include "Application/Application.h"
#include "Performance/Trace.h"
#include "glCraft.h"

int main(int argc, char **argv) {
  std::string savePath = argc > 1 ? argv[1] : "default.glc";

  int result;
  START_TRACE("startup-trace.json");
  {
    Application app;
    END_TRACE();

    START_TRACE("scene-creation-trace.json");

    app.setScene(std::make_shared<Scene>(savePath));
    END_TRACE();

    START_TRACE("runtime-trace.json");
    result = app.run();
  }
  END_TRACE();
  return result;
}
