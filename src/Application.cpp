#include "Application.h"
#include "Assets/ShaderProgram.h"

Application::Application() {
  window = setupWindow();

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


Application &Application::instance() {
  static Application app;
  return app;
}

GLFWwindow *Application::setupWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwSwapInterval(1);

  GLFWwindow *win = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
  glfwMakeContextCurrent(win);
  return win;
}

int Application::run() {
  if (!window) return -1;

  uint32_t vao, vbo, ibo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glm::vec3 vertices[] = {{0, .5, 0}, {-.5, -.5, 0}, {.5, -.5, 0}};

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
  glEnableVertexAttribArray(0);

  uint32_t indices[] = {0, 1, 2};

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  ShaderProgram defaultShader("default", {{0, "position"}});

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();


    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    glm::vec4 clear_color = {0.3f, 0.3f, 0.3f, 1.0f};


    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    defaultShader.bind();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

void GLAPIENTRY Application::OpenGlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                                   GLsizei length, const GLchar *message, const void *userParam) {
  std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << "type = " << type
            << ", severity = " << severity << ", message = " << message << std::endl;
}

void Application::onKeyEvent(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  std::cout << key << std::endl;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::onResized(GLFWwindow *window, int32_t width, int32_t height) {
  auto &app = Application::instance();
  app.windowWidth = width;
  app.windowHeight = height;
}

void Application::onMouseButtonEvent(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
  std::cout << button << " " << action << " " << mods << std::endl;
}


void Application::setupCallbacks(GLFWwindow *window) {
  glfwSetKeyCallback(window, onKeyEvent);
  glfwSetMouseButtonCallback(window, onMouseButtonEvent);
  glfwSetFramebufferSizeCallback(window, onResized);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(OpenGlMessageCallback, nullptr);
}

void Application::setupImgui(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450 core");
}

bool Application::setupGlad() {
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return false;
  }
  return true;
}

Application::~Application() {}
