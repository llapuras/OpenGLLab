#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//global variables
//-----------------------------------
//bool
bool show_demo_window = true;
bool show_another_window = false;
bool show_Mouse = true;
bool isShowShadow = true;

//test
float posx = 0;
float posy = 0;
float posz = 0;

//water
float DRAG_MULT = .02f;

//HeightMap
float heightScale = 0.2f;

//shadow land
float litposx = 0;
float litposy = 0;
float litposz = 0;

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

		//Environment Map
		ImGui::Text("Introduction");
		ImGui::Text("1.Use Arrow Keys to rotate Camera");
		ImGui::Text("2.Use W/A/S/D to move Camera");
		ImGui::Text("slide Wave Size to change water effect");
		ImGui::Text("4.Press Q/E or slide Height Scale to change heightscale of height map");


		//Water Effect
		ImGui::Text("");
		ImGui::Text("Water Effect Parameter:");
		ImGui::SliderFloat("Wave Size", &DRAG_MULT, 0, 0.2);

		//Height Map
		ImGui::Text("");
		ImGui::Text("Height Map:");
		ImGui::SliderFloat("Height Scale", &heightScale, 0, 0.8);


		////for shadow scene Light Pos
		//ImGui::Text("");
		//ImGui::Text("test Pos");
		//ImGui::SliderFloat("posx", &posx, -1, 1);// Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::SliderFloat("posy", &posy, -1, 1);// Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::SliderFloat("posz", &posz, -1, 1);// Edit 1 float using a slider from 0.0f to 1.0f

		////for shadow scene Light Pos
		//ImGui::Text("");
		//ImGui::Text("Shaodw Light Pos");
		//ImGui::SliderFloat("litposx", &litposx, -30, 30);// Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::SliderFloat("litposy", &litposy, -30, 30);// Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::SliderFloat("litposz", &litposz, -30, 30);// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::End();


		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	};

};