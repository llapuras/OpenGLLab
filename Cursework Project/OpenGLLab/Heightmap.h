#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "UI.h"
#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define RAW_WIDTH 257
#define RAW_HEIGHT 257

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f


float heightScale = 0.1f;

unsigned int loadTexture(const char* path);
void renderQuad();

class Heightmap {
public:
	Shader shader;
	glm::vec3 lightPos;
	unsigned int diffuseMap = loadTexture("Images/green.jpg");
	unsigned int normalMap = loadTexture("Images/terrain01normal.jpg");
	unsigned int heightMap = loadTexture("Images/fbm.png");
	unsigned int watertexture = loadTexture("Images/water.jpg");

	unsigned int numVertices = RAW_WIDTH * RAW_HEIGHT;
	unsigned int numIndices = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;
	glm::vec3* vertices = new glm::vec3[numVertices];
	glm::vec2* textureCoords = new glm::vec2[numVertices];
	unsigned int* indices = new unsigned int[numIndices];
	unsigned char* data = new unsigned char[numVertices];

	glm::vec3* normals;
	glm::vec4* colours;
	glm::vec3* tans;

	unsigned int VAO;
	unsigned int VBO[MAX_BUFFER];

	Heightmap() {
		shader = Shader("Shaders/watervs.glsl", "Shaders/waterfs.glsl");
	};

	void RenderWater(Camera camera) {
		shader.use();
		shader.setInt("diffuseMap", 0);
		
		lightPos = glm::vec3(posx, posy, posz);
		// configure view/projection matrices
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// render parallax-mapped quad
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show parallax mapping from multiple directions
		model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0))); // rotate the quad to show parallax mapping from multiple directions
		model = glm::scale(model, glm::vec3(1, 1, 1)); // rotate the quad to show parallax mapping from multiple directions
		model = glm::translate(model, glm::vec3(0, -1, 0));
		shader.setMat4("model", model);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setFloat("heightScale", heightScale); // adjust with Q and E keys
		shader.setFloat("time", glfwGetTime());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, watertexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		renderQuad();
	}

	void renderHeightMap() {
	
		//装载顶点和贴图坐标
		for (int x = 0; x < RAW_WIDTH; ++x) {
			for (int z = 0; z < RAW_HEIGHT; ++z) {
				int offset = (x * RAW_WIDTH) + z;

				vertices[offset] = glm::vec3(x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);
				textureCoords[offset] = glm::vec2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
			}
		}

		delete data;
		numIndices = 0;

		for (int x = 0; x < RAW_WIDTH - 1; ++x) {
			for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
				int a = (x * (RAW_WIDTH)) + z;
				int b = ((x + 1) * (RAW_WIDTH)) + z;
				int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
				int d = (x * (RAW_WIDTH)) + (z + 1);

				indices[numIndices++] = c;
				indices[numIndices++] = b;
				indices[numIndices++] = a;

				indices[numIndices++] = a;
				indices[numIndices++] = d;
				indices[numIndices++] = c;
			}
		}

		GenerateNormals();
		BufferData();
		glBindVertexArray(VAO);
		//if (VBO[INDEX_BUFFER]) {
		//	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
		//}
		//else {
		//	glDrawArrays(GL_TRIANGLES, 0, numVertices);
		//}

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	};


	void GenerateNormals() {
		if (!normals) {
			normals = new glm::vec3[numVertices];
		}
		for (GLuint i = 0; i < numVertices; ++i) {
			normals[i] = glm::vec3();
		}
		if (indices) { // Generate per - vertex normals
			for (GLuint i = 0; i < numIndices; i += 3) {
				unsigned int a = indices[i];
				unsigned int b = indices[i + 1];
				unsigned int c = indices[i + 2];

				glm::vec3 normal = glm::cross((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));

				normals[a] += normal;
				normals[b] += normal;
				normals[c] += normal;
			}
		}
		else { // It ’s just a list of triangles , so generate face normals
			for (GLuint i = 0; i < numVertices; i += 3) {
				glm::vec3& a = vertices[i];
				glm::vec3& b = vertices[i + 1];
				glm::vec3& c = vertices[i + 2];

				glm::vec3 normal = glm::cross(b - a, c - a);

				normals[i] = normal;
				normals[i + 1] = normal;
				normals[i + 2] = normal;
			}
		}

		for (GLuint i = 0; i < numVertices; ++i) {
			normals[i] = glm::normalize(normals[i]);
		}
	};


	void GenerateTans() {
		if (!tans) {
			tans = new glm::vec3[numVertices];
		}
		for (GLuint i = 0; i < numVertices; ++i) {
			tans[i] = glm::vec3();
		}
		if (indices) { // Generate per - vertex normals
			for (GLuint i = 0; i < numIndices; i += 3) {
				unsigned int a = indices[i];
				unsigned int b = indices[i + 1];
				unsigned int c = indices[i + 2];

				glm::vec3 normal = glm::cross((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));

				tans[a] += normal;
				tans[b] += normal;
				tans[c] += normal;
			}
		}
		else { // It ’s just a list of triangles , so generate face normals
			for (GLuint i = 0; i < numVertices; i += 3) {
				glm::vec3& a = vertices[i];
				glm::vec3& b = vertices[i + 1];
				glm::vec3& c = vertices[i + 2];

				glm::vec3 normal = glm::cross(b - a, c - a);

				normals[i] = normal;
				normals[i + 1] = normal;
				normals[i + 2] = normal;
			}
		}

		for (GLuint i = 0; i < numVertices; ++i) {
			normals[i] = glm::normalize(normals[i]);
		}
	};


	void BufferData() {
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_BUFFER);

		if (normals) {
			glGenBuffers(1, &VBO[NORMAL_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
			glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(NORMAL_BUFFER);
		}

		if (textureCoords) { // This bit is new !
			glGenBuffers(1, &VBO[TEXTURE_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[TEXTURE_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec2), textureCoords, GL_STATIC_DRAW);
			glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(TEXTURE_BUFFER);
		}

		if (colours) { // Just in case the data has no colour attribute ...
			glGenBuffers(1, &VBO[COLOUR_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[COLOUR_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec2), colours, GL_STATIC_DRAW);
			glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(COLOUR_BUFFER);
		}

		if (indices) {
			glGenBuffers(1, &VBO[INDEX_BUFFER]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFFER]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
	}



	void Render(Camera camera) {
		shader.use();
		shader.setInt("diffuseMap", 0);
		shader.setInt("normalMap", 1);
		shader.setInt("depthMap", 2);

		lightPos = glm::vec3(posx, posy, posz);

		// configure view/projection matrices
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// render parallax-mapped quad
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show parallax mapping from multiple directions
		model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0))); // rotate the quad to show parallax mapping from multiple directions
		model = glm::scale(model, glm::vec3(10, 10, 10)); // rotate the quad to show parallax mapping from multiple directions
		model = glm::translate(model, glm::vec3(1, -0.3, 0));
		shader.setMat4("model", model);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setFloat("heightScale", heightScale); // adjust with Q and E keys
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		renderQuad();

		//// render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.1f));
		//shader.setMat4("model", model);
		//renderQuad();

	}


};


// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 0.0f, 1.0f);
		glm::vec3 pos2(-1.0f, 0.0f, -1.0f);
		glm::vec3 pos3(1.0f, 0.0f, -1.0f);
		glm::vec3 pos4(1.0f, 0.0f, 1.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 1.0f, 0.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}