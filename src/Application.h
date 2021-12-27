#pragma once

#include "glCraft.h"

class Application {
private:
  const char *name = "glCraft";
  int windowWidth = 800;
  int windowHeight = 600;
  GLFWwindow *window = nullptr;

  Application();

  GLFWwindow *setupWindow();
  static void setupCallbacks(GLFWwindow *window);
  static void setupImgui(GLFWwindow *pWwindow);
  static bool setupGlad();

  static void onKeyEvent(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode);
  static void onResized(GLFWwindow *window, int32_t width, int32_t height);
  static void onMouseButtonEvent(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);

public:
  static Application &instance();

  Application(const Application &) = delete;
  Application(Application &) = delete;
  Application(Application &&) = delete;

  int run();

  virtual ~Application();
  static void OpenGlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                             const GLchar *message, const void *userParam);
};
