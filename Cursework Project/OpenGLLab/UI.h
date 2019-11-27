#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//parameters
bool show_demo_window = true;
bool show_another_window = false;
bool show_Mouse = true;
bool isShowShadow = true;
float posx = 0;
float posy = 0;
float posz = 0;
float xconstant = 1.0f;
float xlinear = 0.09f;
float xquadratic = 0.032f;

class UI {
public:

	UI(GLFWwindow* window) {
		ImGui::CreateContext();
		IMGUI_CHECKVERSION();
		ImGuiIO& io = ImGui::GetIO();  (void)io;
		io.MouseDrawCursor = true;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};

	~UI() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Render(GLFWwindow* window) {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("awsl");// Create a window called "Hello, world!" and append into it.s
		ImGui::Checkbox("Mouse Control", &show_another_window);
		ImGui::SliderFloat("posx", &posx, -22, 22);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("posy", &posy, -22, 22);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("posz", &posz, -22, 22);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::End();

		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	};

};