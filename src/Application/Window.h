#pragma once

#include "../glCraft.h"

class Window {
  const char *name = "glCraft";
  int32_t windowWidth = 800;
  int32_t windowHeight = 600;
  GLFWwindow *window = nullptr;
  glm::vec4 clearColor = {0, 0, 0, 1};

  Window();

  void setupCallbacks();
  static bool setupGlad();

  static void onKeyEvent(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode);
  static void onResized(GLFWwindow *window, int32_t width, int32_t height);
  static void onMouseButtonEvent(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
  static void onCursorPosition(GLFWwindow *window, double x, double y);
  static void onRefreshWindow(GLFWwindow *window);

  static void onWindowError(int32_t errorCode, const char *description);
  static void onOpenGlMessage(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length,
                              const GLchar *message,
                              const void *userParam);

public:
  Window(const Window &) = delete;
  Window(Window &) = delete;
  Window(Window &&) = delete;

  [[nodiscard]] inline int32_t getWindowWidth() const { return windowWidth; }
  void setWindowWidth(int32_t width) { windowWidth = width; }

  [[nodiscard]] inline int32_t getWindowHeight() const { return windowHeight; }
  void setWindowHeight(int32_t height) { windowHeight = height; }

  [[nodiscard]] inline GLFWwindow *getContext() { return window; };

  static Window &instance() {
    static Window window;
    return window;
  }
  bool isValid() { return window != nullptr; };
  [[nodiscard]] inline bool shouldClose() const { return glfwWindowShouldClose(window); };
  void update();
  void finalizeFrame();

  ~Window();
  void pollEvents();
  void unlockMouse();
  void lockMouse();
  glm::dvec2 getCursorPosition();
};
