///*add shadow map*/
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Shader.h"
//#include "Model.h"
//#include "Camera.h"
//#include "Geometry.h"
//#include "SkyBox.h"
//#include "Light.h"
//#include "SceneNode.h"
//#include <iostream>
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//GLuint loadTexture(const char* path);
//void RenderScene(SceneNode* root, Shader& shader);
//void RenderQuad();
//void RenderCube();
//void createEnvirnCube();
//
//Camera camera(glm::vec3(2.0f, 7.0f, 0.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//glm::vec3 pointpos = glm::vec3(0.0f, 0.0f, 0.0f);
//
//const char* glsl_version = "#version 330";
//
//SkyBox sky;
//Geometry plane;
//
//Light light01;
//DirectionLight dirLight;
//PointLight pointLight;
//SpotLight spotLight;
//
//// Global variables
//GLuint woodTexture;
//GLuint planeVAO;
//
//int main()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LapuEngine(´-w-`)", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	glfwSetScrollCallback(window, scroll_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	// Define the viewport dimensions
//	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//	glEnable(GL_DEPTH_TEST);
//
//	double lastTime = glfwGetTime();
//	int nbFrames = 0;
//
//	//shader
//	Shader ourShader("Shaders/modelload_vs.glsl", "Shaders/modelload_fs.glsl");
//	Shader skyboxShader("Shaders/skyboxvs.glsl", "Shaders/skyboxfs.glsl");
//	Shader simpleDepthShader("Shaders/shadowvs.glsl", "Shaders/shadowfs.glsl");
//	Shader shader("Shaders/shadowmapvs.glsl", "Shaders/shadowmapfs.glsl");
//	Shader environShader("Shaders/environmentvs.glsl", "Shaders/environmentfs.glsl");
//
//	//cw
//	Model land("Model/land/land.obj");
//	Model bush("Model/land/bush.obj");
//	Model campfire("Model/land/campfire.obj");
//	Model mushroom("Model/land/mushroom.obj");
//	Model tree("Model/land/tree.obj");
//	Model castle("Model/land/monu9.obj");
//	Model lamp("Model/land/lamp01.obj");
//
//	ImGui::CreateContext();
//	IMGUI_CHECKVERSION();
//	ImGuiIO& io = ImGui::GetIO();  (void)io;
//	io.MouseDrawCursor = true;
//	ImGui::StyleColorsDark();
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init(glsl_version);
//	bool show_demo_window = true;
//	bool show_another_window = false;
//	bool show_Mouse = true;
//	bool isShowShadow = true;
//	float posx = 0;
//	float posy = 0;
//	float posz = 0;
//
//	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//	//model pos array
//	int amount = 1000;
//	glm::mat4* modelMatrices;
//	modelMatrices = new glm::mat4[amount];
//	srand(glfwGetTime()); // initialize random seed
//	GLfloat radius = 50.0;
//	GLfloat offset = 2.5f;
//	for (int i = 0; i < amount; i++)
//	{
//		glm::mat4 model;
//		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
//		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
//		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
//		GLfloat x = sin(angle) * radius + displacement;
//		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
//		GLfloat y = displacement * 0.4f; // y value has smaller displacement
//		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
//		GLfloat z = cos(angle) * radius + displacement;
//		model = glm::translate(model, glm::vec3(x, y, z));
//		// 2. Scale: Scale between 0.05 and 0.25f
//		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
//		model = glm::scale(model, glm::vec3(scale));
//		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
//		GLfloat rotAngle = (rand() % 360);
//		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
//		// 4. Now add to list of matrices
//		modelMatrices[i] = model;
//	}
//
//	//skybox
//	sky.InitSkyBox(skyboxShader);
//
//	//initial scene
//	SceneNode* root = new SceneNode(land, "gameobject");
//	SceneNode* bush_child = new SceneNode(bush, "bush");
//	SceneNode* tree_child = new SceneNode(tree, "tree");
//	SceneNode* tree_child01 = new SceneNode(tree, "tree01");
//	SceneNode* campfire_child = new SceneNode(campfire, "campfire");
//	SceneNode* mushroom_child01 = new SceneNode(mushroom, "mushroom01");
//	SceneNode* mushroom_child02 = new SceneNode(mushroom, "mushroom02");
//	SceneNode* mushroom_child03 = new SceneNode(mushroom, "mushroom03");
//	SceneNode* castle_child = new SceneNode(castle, "land");
//	SceneNode* lamp_child = new SceneNode(lamp, "lamp");
//
//	root->AddChild(bush_child);
//	root->AddChild(tree_child);
//	root->AddChild(tree_child01);
//	root->AddChild(campfire_child);
//	root->AddChild(mushroom_child01);
//	root->AddChild(mushroom_child02);
//	root->AddChild(mushroom_child03);
//	root->AddChild(castle_child);
//	root->AddChild(lamp_child);
//
//	//------------------------------------------------------------
//	//shadow!!!!fighting!!!
//	//plane
//	GLfloat planeVertices[] = {
//		// Positions          // Normals         // Texture Coords
//		25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
//		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
//		-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//
//		25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
//		25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
//		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
//	};
//	// Setup plane VAO
//	GLuint planeVBO;
//	glGenVertexArrays(1, &planeVAO);
//	glGenBuffers(1, &planeVBO);
//	glBindVertexArray(planeVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//	glBindVertexArray(0);
//
//	// Load textures
//	woodTexture = loadTexture("Images/wall1.jpg");
//
//	// Configure depth map FBO
//	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
//	GLuint depthMapFBO;
//	glGenFramebuffers(1, &depthMapFBO);
//	// - Create depth texture
//	GLuint depthMap;
//	glGenTextures(1, &depthMap);
//	glBindTexture(GL_TEXTURE_2D, depthMap);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// Set texture samples
//	shader.use();
//	shader.setInt("diffuseTexture", 0);
//	shader.setInt("shadowMap", 1);
//
//	// Light source
//	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
//
//	// --------------------
//	environShader.use();
//	environShader.setInt("skybox", 0);
//
//	while (!glfwWindowShouldClose(window))
//	{
//
//		double currentTime = glfwGetTime();
//		nbFrames++;
//		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
//		 // printf and reset timer
//			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
//			nbFrames = 0;
//			lastTime += 1.0;
//		}
//
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		processInput(window);
//
//		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		ourShader.use();
//		ourShader.setVec3("viewPos", camera.Position);
//		ourShader.setFloat("material.shininess", 32.0f);
//
//		//Light
//		light01.RenderDirLight(ourShader, dirLight);
//
//		pointLight.position = glm::vec3(posx, posy, posz);
//		light01.RenderPointLight(ourShader, pointLight);
//
//		spotLight.position = camera.Position;
//		spotLight.direction = camera.Front;
//		light01.RenderSpotLight(ourShader, spotLight);
//
//
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glm::mat4 view = camera.GetViewMatrix();
//		glm::mat4 model = glm::mat4(1.0f);
//
//
//		for (GLuint i = 0; i < amount; i++)
//		{
//			//glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
//			ourShader.setMat4("model", (modelMatrices[i]));
//			mushroom.Draw(ourShader);
//		}
//
//		// draw scene as normal
//		environShader.use();
//		model = glm::mat4(1.0f);
//		view = camera.GetViewMatrix();
//		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		environShader.setMat4("model", model);
//		environShader.setMat4("view", view);
//		environShader.setMat4("projection", projection);
//		shader.setVec3("cameraPos", camera.Position);
//		// cubes
//		createEnvirnCube();
//
//		//normal
//		glm::mat4 lightProjection, lightView;
//		glm::mat4 lightSpaceMatrix;
//		GLfloat near_plane = 1.0f, far_plane = 7.5f;
//		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
//		lightSpaceMatrix = lightProjection * lightView;
//		simpleDepthShader.use();
//		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
//
//		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//		glClear(GL_DEPTH_BUFFER_BIT);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, woodTexture);
//		RenderScene(root, simpleDepthShader);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//		//shader on--------------------------------------------------------------
//		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		shader.use();
//		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		view = camera.GetViewMatrix();
//		shader.setMat4("projection", projection);
//		shader.setMat4("view", view);
//		shader.setVec3("viewPos", camera.Position);
//		shader.setVec3("lightPos", lightPos);
//		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, woodTexture);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, depthMap);
//		if (isShowShadow) {
//			RenderScene(root, shader);
//		}
//
//		//(posx, posy, posz);
//		castle_child->modelScale = glm::vec3(2, 2, 2);
//		castle_child->transform = glm::vec3(-1, -3, 0);
//		tree_child->transform = glm::vec3(-2, 0, -3);
//		tree_child01->transform = glm::vec3(3, 0, -2);
//		bush_child->transform = glm::vec3(6, 0, -4);
//		campfire_child->transform = glm::vec3(2, 0, -11);
//		mushroom_child01->transform = glm::vec3(-2, 0, -10);
//		mushroom_child02->transform = glm::vec3(4, 0, -5);
//		mushroom_child03->transform = glm::vec3(3, 0, -2);
//		lamp_child->transform = glm::vec3(-2.3f, 2.0f, -4.5f);
//		//root->worldTransform = glm::vec3(posx, posy, posz);
//		root->Draw(root, camera, ourShader);
//
//		//draw sky box
//		sky.DrawSkyBox(camera, skyboxShader);
//
//		//ui
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//		if (show_demo_window)
//			ImGui::ShowDemoWindow(&show_demo_window);
//		if (show_another_window)
//			glfwSetCursorPosCallback(window, mouse_callback);
//		{
//			static float f = 0.0f;
//			static int counter = 0;
//
//			ImGui::Begin("Hello, world!");// Create a window called "Hello, world!" and append into it.s
//
//			ImGui::Text("This is some useful text.");// Display some text (you can use a format strings too)
//			ImGui::Checkbox("Demo Window", &show_demo_window);// Edit bools storing our window open/close state
//			ImGui::Checkbox("Mouse Control", &show_another_window);
//			ImGui::Checkbox("Show Shadow", &isShowShadow);
//			ImGui::SliderFloat("posx", &posx, -10.0f, 10.0f);// Edit 1 float using a slider from 0.0f to 1.0f
//			ImGui::SliderFloat("posy", &posy, -10.0f, 10.0f);// Edit 1 float using a slider from 0.0f to 1.0f
//			ImGui::SliderFloat("posz", &posz, -10.0f, 10.0f);// Edit 1 float using a slider from 0.0f to 1.0f
//
//			ImGui::ColorEdit3("clear color", (float*)&f);// Edit 3 floats representing a color
//
//			if (ImGui::Button("Button"))// Buttons return true when clicked (most widgets return true when edited/activated)
//				counter++;
//			ImGui::SameLine();
//			ImGui::Text("counter = %d", counter);
//
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//			ImGui::Text("fps = %.3f", 1000.0 / double(nbFrames));
//
//			ImGui::SliderInt("amount", &amount, 0, 5000);// Edit 1 float using a slider from 0.0f to 1.0f
//
//			ImGui::End();
//		}
//
//		ImGui::Render();
//		int display_w, display_h;
//		glfwMakeContextCurrent(window);
//		glfwGetFramebufferSize(window, &display_w, &display_h);
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//
//	sky.DeleteSkybox();
//
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//	glfwTerminate();
//	return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		camera.ProcessKeyboard(FORWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		camera.ProcessKeyboard(BACKWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		camera.ProcessKeyboard(LEFT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		camera.ProcessKeyboard(RIGHT, deltaTime);
//
//	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//		camera.ProcessKeyRotate(ROTATE_RIGHT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//		camera.ProcessKeyRotate(ROTATE_LEFT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//		camera.ProcessKeyRotate(ROTATE_UP, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//		camera.ProcessKeyRotate(ROTATE_DOWN, deltaTime);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(yoffset);
//}
//
//void RenderScene(SceneNode* root, Shader& shader)
//{
//	root->Draw(root, camera, shader);
//}
//
//
//unsigned int loadTexture(char const* path)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}
//
//
//void createEnvirnCube() {
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//// ------------------------------------------------------------------
//	float cubeVertices[] = {
//		// positions          // normals
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//
//		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//	};
//
//	// cube VAO
//	unsigned int cubeVAO, cubeVBO;
//	glGenVertexArrays(1, &cubeVAO);
//	glGenBuffers(1, &cubeVBO);
//	glBindVertexArray(cubeVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	// skybox VAO
//
//	glBindVertexArray(cubeVAO);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, sky.SkyboxTexture);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//
//}