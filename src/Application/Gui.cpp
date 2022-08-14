#include "Gui.h"

#include "../Performance/Trace.h"
#include "Window.h"

Gui *Gui::instancePtr = nullptr;

Gui::Gui() {
  TRACE_FUNCTION();
  assert(instancePtr == nullptr && "The GUI is already instantiated");
  instancePtr = this;

  const auto context = Window::instance().getContext();
  if (context == nullptr) {
    return;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(context, true);
  ImGui_ImplOpenGL3_Init("#version 450 core");
}

Gui::~Gui() {
  TRACE_FUNCTION();
  instancePtr = nullptr;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Gui::beginFrame() {
  TRACE_FUNCTION();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Gui::finalizeFrame() {
  TRACE_FUNCTION();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
