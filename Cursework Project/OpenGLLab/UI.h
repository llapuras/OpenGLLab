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
float litposx = 0;
float litposy = 0;
float litposz = 0;
float posx = 0;
float posy = 0;
float posz = 0;
float xconstant = 1.0f;
float xlinear = 0.09f;
float xquadratic = 0.032f;

//water
float DRAG_MULT = .02f;

//HeightMap
float heightScale = 0.2f;


bool f = false;

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


		ImGui::Begin("Menu");

		//Mouse Control
		ImGui::Checkbox("Mouse Control", &show_another_window);

		//Water Effect
		ImGui::Text("Water Effect Parameter:");
		ImGui::SliderFloat("Wave Size", &DRAG_MULT, 0, 0.2);

		//Height Map
		ImGui::Text("");
		ImGui::Text("Height Map:");
		ImGui::SliderFloat("Height Scale", &heightScale, 0, 0.8);

		//Environment Map
		ImGui::Text("");
		ImGui::Text("Environment Map");
		ImGui::Text("Sky Box -> Cubemap");
		ImGui::Text("Plane -> Reflection");
		ImGui::Text("Castle -> Fraction");

		//Scene Graph
		ImGui::Text("");
		ImGui::Text("Scene Graph:");

		//Shadow Map
		ImGui::Text("");
		ImGui::Text("Scene Graph:");

		//Scene Graph
		ImGui::Text("");
		ImGui::Text("Scene Graph:");


		//for shadow scene Light Pos
		ImGui::Text("");
		ImGui::Text("Shaodw Light Pos");
		ImGui::SliderFloat("posx", &litposx, -1, 1);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("posy", &litposy, -1, 1);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("posz", &litposz, -1, 1);// Edit 1 float using a slider from 0.0f to 1.0f

		//for shadow scene Light Pos
		ImGui::Text("");
		ImGui::Text("Shaodw Light Pos");
		ImGui::SliderFloat("posx", &posx, -50, 50);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("posy", &posy, -50, 50);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("posz", &posz, -50, 50);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::End();


		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	};

};