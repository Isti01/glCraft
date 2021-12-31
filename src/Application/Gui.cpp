#include "Gui.h"

#include "Window.h"

Gui::Gui() {
  const auto context = Window::instance().getContext();
  if (context == nullptr) return;
  std::cout << "initialized the gui" << std::endl;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(context, true);
  ImGui_ImplOpenGL3_Init("#version 450 core");
}

void Gui::update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Gui::finalizeFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Gui::~Gui() {
  std::cout << "gui destroyed" << std::endl;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
