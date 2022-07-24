#pragma once

#include "../Rendering/FramebufferStack.h"
#include "../glCraft.h"

class Window {
  static Window *instancePtr;

  const char *name = "glCraft";
  int32_t windowWidth = 1200;
  int32_t windowHeight = 900;
  GLFWwindow *window = nullptr;
  glm::vec4 clearColor = {0, 0, 0, 1};
  Ref<FramebufferStack> framebufferStack = std::make_shared<FramebufferStack>();

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
  Window();
  ~Window();

  static Window &instance() { return *instancePtr; }

  [[nodiscard]] inline int32_t getWindowWidth() const { return windowWidth; }
  void setWindowWidth(int32_t width) { windowWidth = width; }

  [[nodiscard]] inline int32_t getWindowHeight() const { return windowHeight; }
  void setWindowHeight(int32_t height) { windowHeight = height; }

  [[nodiscard]] inline GLFWwindow *getContext() { return window; };
  [[nodiscard]] inline Ref<FramebufferStack> getFramebufferStack() { return framebufferStack; };

  bool isValid() { return window != nullptr; };
  [[nodiscard]] inline bool shouldClose() const { return glfwWindowShouldClose(window); };

  void beginFrame();
  void resetFrame();
  void finalizeFrame();
  void swapBuffers();

  void pollEvents();
  void unlockMouse();
  void lockMouse();
  glm::dvec2 getCursorPosition();

  Window(const Window &) = delete;
  Window(Window &) = delete;
  Window(Window &&) noexcept = delete;
  Window &operator=(Window &) = delete;
  Window &operator=(Window &&) noexcept = delete;
  bool shouldRender();
};
