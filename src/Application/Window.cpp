#include "Window.h"
#include "Application.h"

Window::Window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwSwapInterval(1);

  window = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  if (!setupGlad()) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    window = nullptr;
    glfwTerminate();
    return;
  }

  setupCallbacks(window);
  setupImgui(window);
}

void GLAPIENTRY Window::onOpenGlMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                        const GLchar *message, const void *userParam) {
  std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << "type = " << type
            << ", severity = " << severity << ", message = " << message << std::endl;
}

void Window::onKeyEvent(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  std::cout << key << std::endl;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void Window::onResized(GLFWwindow *_, int32_t width, int32_t height) {
  Window &window = Application::instance().getWindow();
  window.setWindowHeight(height);
  window.setWindowWidth(width);
}

void Window::onMouseButtonEvent(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
  std::cout << button << " " << action << " " << mods << std::endl;
}

void Window::setupCallbacks(GLFWwindow *window) {
  glfwSetKeyCallback(window, onKeyEvent);
  glfwSetMouseButtonCallback(window, onMouseButtonEvent);
  glfwSetFramebufferSizeCallback(window, onResized);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(onOpenGlMessage, nullptr);
}

void Window::setupImgui(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450 core");
}

bool Window::setupGlad() {
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return false;
  }
  return true;
}
void Window::update() {
  glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
  glViewport(0, 0, windowWidth, windowHeight);
}
void Window::swapBuffers() { glfwSwapBuffers(window); }
