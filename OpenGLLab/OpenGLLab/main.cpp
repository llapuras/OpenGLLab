#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Geometry.h"
#include "SkyBox.h"
#include "Light.h"


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

Camera camera(glm::vec3(-0.5f, 2.0f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 pointpos = glm::vec3(0.0f, 0.0f, 0.0f);

const char* glsl_version = "#version 330";

SkyBox sky;
Geometry plane;

Light light01;
DirectionLight dirLight;
PointLight pointLight;
SpotLight spotLight;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LapuEngine(´-w-`)", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("Shaders/modelload_vs.glsl", "Shaders/modelload_fs.glsl");
	Shader skyboxShader("Shaders/skyboxvs.glsl", "Shaders/skyboxfs.glsl");
	Shader instancedShader("Shaders/skyboxvs.glsl", "Shaders/skyboxfs.glsl");

	//Model ourModel02("Model/character/nanosuit.obj");
	Model lamp("Model/lamp/lamp01.obj");
	Model sponge("Model/spongebob/one-room-pratamacam.obj");

	ImGui::CreateContext();
	IMGUI_CHECKVERSION();
	ImGuiIO& io = ImGui::GetIO();  (void)io;
	io.MouseDrawCursor = true;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_Mouse = true;
	float posx = 0;
	float posy = 0;
	float posz = 0;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	//model pos array
	GLuint amount = 1000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed
	GLfloat radius = 50.0;
	GLfloat offset = 2.5f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.4f; // y value has smaller displacement
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}


	sky.InitSkyBox(skyboxShader);

	////position list
	//glm::vec3 translations[1000];
	//int index = 0;
	//GLfloat offset = 0.1f;
	//for (GLint y = -10; y < 10; y += 2)
	//{
	//	for (GLint x = -10; x < 10; x += 2)
	//	{
	//		for (GLint z = -10; z < 10; z += 2) {
	//			glm::vec3 translation;
	//			translation.x = (GLfloat)x / 10.0f + offset;
	//			translation.y = (GLfloat)y / 10.0f + offset;
	//			translation.z = (GLfloat)z / 10.0f + offset;
	//			translations[index] = translation;
	//			index++;
	//		}
	//	}
	//}

	////send position to shader
	//instancedShader.use();
	//glm::vec3 newpos;
	//for (GLuint i = 0; i < 100; i++)
	//{
	//	stringstream ss;
	//	string index;
	//	ss << i;
	//	index = ss.str();
	//	newpos = glm::vec3(translations[i].x, translations[i].y, translations[i].z);
	//	instancedShader.setVec3(("offsets[" + index + "]"), newpos);
	//}

	//GLuint instanceVBO;
	//glGenBuffers(1, &instanceVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, &translations[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glVertexAttribDivisor(2, 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("material.shininess", 32.0f);

		//Light
		light01.RenderDirLight(ourShader, dirLight);
		light01.RenderPointLight(ourShader, pointLight);
		spotLight.position = camera.Position;
		spotLight.direction = camera.Front;
		light01.RenderSpotLight(ourShader, spotLight);

		////Model
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::translate(model, glm::vec3(posx, posy, posz)); // translate it down so it's at the center of the scene
		ourShader.setMat4("model", model);
		//plane.DrawPlane(camera, ourShader);
		sponge.Draw(ourShader);

		//glm::mat4 model = glm::mat4(1.0f);
		//for (int x = 0; x < 3; x++) {
		//	for(int y = 0;y<3;y++){
		//		for (int z = 0; z < 3; z++) {
		//			model = glm::mat4(1.0f);
		//			model = glm::translate(model, glm::vec3(x-3, y-3, z)); // translate it down so it's at the center of the scene
		//			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//			ourShader.setMat4("model", model);
		//			lamp.Draw(ourShader);
		//		}
		//	}
		//}



		// Draw meteorites
		for (GLuint i = 0; i < amount; i++)
		{
			//glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
			ourShader.setMat4("model", (modelMatrices[i]));
			lamp.Draw(ourShader);
		}





		sky.DrawSkyBox(camera, skyboxShader);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		if (show_another_window)
			glfwSetCursorPosCallback(window, mouse_callback);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");// Create a window called "Hello, world!" and append into it.s

			ImGui::Text("This is some useful text.");// Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);// Edit bools storing our window open/close state
			ImGui::Checkbox("Mouse Control", &show_another_window);

			ImGui::SliderFloat("posx", &posx, -10.0f, 10.0f);// Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("posy", &posy, -10.0f, 10.0f);// Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("posz", &posz, -10.0f, 10.0f);// Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::ColorEdit3("clear color", (float*)&f);// Edit 3 floats representing a color

			if (ImGui::Button("Button"))// Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	sky.DeleteSkybox();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyRotate(ROTATE_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyRotate(ROTATE_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyRotate(ROTATE_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyRotate(ROTATE_DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
