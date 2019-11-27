#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MeshX.h"
class Renderer{
public:
	Renderer(GLFWwindow* window) {
		triangle = MeshX::GenerateTriangle();
		quad = MeshX::GenerateQuad();
		//skyboxShader = Shader("Shaders/skyboxvs.glsl", "Shaders/skyboxfs.glsl");

		//sky.InitSkyBox(skyboxShader);
	}
	
	virtual ~Renderer(void) {
		delete triangle;
		delete quad;
		//sky.DeleteSkybox();
	}

	virtual void RenderScene() {
		triangle->Draw();
		quad->Draw();
		//sky.DrawSkyBox(camera, skyboxShader);
	}

protected:
	MeshX* triangle;
	MeshX* quad;
	//SkyBox sky;
	//Shader skyboxShader;
};
