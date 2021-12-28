#pragma once

class Window {
  const char *name = "glCraft";
  int windowWidth = 800;
  int windowHeight = 600;
  GLFWwindow *window = nullptr;

  Window();

  static void setupCallbacks(GLFWwindow *window);
  static void setupImgui(GLFWwindow *window);
  static bool setupGlad();

  static void onKeyEvent(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode);
  static void onResized(GLFWwindow *window, int32_t width, int32_t height);
  static void onMouseButtonEvent(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
  static void onOpenGlMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam);

public:
  [[nodiscard]] int getWindowWidth() const { return windowWidth; }
  void setWindowWidth(int windowWidth) { Window::windowWidth = windowWidth; }

  [[nodiscard]] int getWindowHeight() const { return windowHeight; }
  void setWindowHeight(int windowHeight) { Window::windowHeight = windowHeight; }

  static Window &instance() {
    static Window window;
    return window;
  }
  bool isValid() { return window != nullptr; };
  bool shouldClose() const { return glfwWindowShouldClose(window); };
  void update();
  void swapBuffers();
};
