#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Geometry
{
public:
	GLuint planeVAO;
	GLuint planeVBO;
	//GLuint woodTexture = loadTexture("../../../resources/textures/wood.png");
	glm::vec3 color = glm::vec3(0.0f,0.5f,0.5f);

	void CreatePlane() {

		GLfloat planeVertices[] = {
			// Positions          // Normals         // Texture Coords
			25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
			-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
			-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

			25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
			25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
			-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
		};

		// Setup plane VAO

		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}

	void DrawPlane(Camera camera, Shader shader) {

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		shader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(glm::mat4(12.0f), 0.0f, glm::vec3(12.0));
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		glBindVertexArray(planeVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1000);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

	}

	void DeletePlane() {
		glDeleteVertexArrays(1, &planeVAO);
		glDeleteBuffers(1, &planeVBO);
	}

	void CreateCube() {

		GLfloat cubeVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
	}

	void CreateSphere() {

	}

	void CreateCylinder() {

	}

};

