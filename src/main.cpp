// glad must be included first
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fstream>
#include <iostream>
#include <sstream>

void mouse_button_callback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
void key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode);
void window_resize_callback(GLFWwindow *window, int32_t width, int32_t height);

std::string readFile(const std::string &path);
uint32_t compileShaderSource(const std::string &path, uint32_t type);
uint32_t createShader(const std::string &name);

int32_t display_width = 800, display_height = 600;

int main() {
  std::cout << "Hello World from glCraft!" << std::endl;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(display_width, display_height, "glCraft", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetFramebufferSizeCallback(window, window_resize_callback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450");

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

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

  uint32_t defaultShader = createShader("default");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();


    glfwGetFramebufferSize(window, &display_width, &display_height);
    glViewport(0, 0, display_width, display_height);
    glm::vec4 clear_color = {0.3f, 0.3f, 0.3f, 1.0f};


    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(defaultShader);
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

uint32_t createShader(const std::string &name) {
  uint32_t vertexShader = compileShaderSource("assets/" + name + ".vert", GL_VERTEX_SHADER);
  uint32_t fragmentShader = compileShaderSource("assets/" + name + ".frag", GL_FRAGMENT_SHADER);
  uint32_t defaultShader = glCreateProgram();

  glBindAttribLocation(defaultShader, 0, "position");

  glAttachShader(defaultShader, fragmentShader);
  glAttachShader(defaultShader, vertexShader);

  glLinkProgram(defaultShader);

  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  return defaultShader;
}

uint32_t compileShaderSource(const std::string &path, uint32_t type) {
  uint32_t shader = glCreateShader(type);
  const std::string vertexShaderSource = readFile(path);
  std::cout << vertexShaderSource << std::endl;
  const char *vertexShaderSourceStr = vertexShaderSource.c_str();
  glShaderSource(shader, 1, &vertexShaderSourceStr, nullptr);
  glCompileShader(shader);

  int32_t success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[2048];
    glGetShaderInfoLog(shader, sizeof(infoLog) / sizeof(infoLog[0]), nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    return 0;
  }

  return shader;
}

std::string readFile(const std::string &path) {
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(path);

  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

void key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mode) {
  std::cout << key << std::endl;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void window_resize_callback(GLFWwindow *window, int32_t width, int32_t height) {
  display_width = width;
  display_height = height;
}

void mouse_button_callback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
  std::cout << button << " " << action << " " << mods << std::endl;
}
