#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum MeshBuffer {
	VERTEX_BUFFER, NORMAL_BUFFER, TEXTURE_BUFFER, COLOUR_BUFFER,  INDEX_BUFFER, MAX_BUFFER
};

class MeshX {
public:
	MeshX() {
		for (int i = 0; i < MAX_BUFFER; ++i) {
			VBO[i] = 0;
		}
		glGenVertexArrays(1, &VAO);

		numVertices = 0;
		vertices = NULL;
		colours = NULL;
		type = GL_TRIANGLES;

		numIndices = 0;
		indices = NULL;
		normals = NULL;
	};

	~MeshX(void) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(MAX_BUFFER, VBO);

		texture = 0;
		textureCoords = NULL;

		glDeleteTextures(1, &texture);

		delete[] textureCoords;
		delete[] vertices;
		delete[] colours;
		delete[] indices;
		delete[] normals;
	};

	void Draw() {
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		if (VBO[INDEX_BUFFER]) {
			glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(type, 0, numVertices);
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static MeshX* GenerateTriangle() {
		MeshX* m = new MeshX();
		m->numVertices = 6;

		m->vertices = new glm::vec3[m->numVertices];
		m->vertices[0] = glm::vec3(0.0f, 0.5f, 0.0f);
		m->vertices[1] = glm::vec3(0.5f, 0.5f, 0.0f);
		m->vertices[2] = glm::vec3(0.5f, 0.f, 0.f);
		m->vertices[3] = glm::vec3(0.0f, -0.5f, 0.0f);
		m->vertices[4] = glm::vec3(-0.5f, -0.5f, 0.0f);
		m->vertices[5] = glm::vec3(-0.5f, 0.f, 0.f);

		m->textureCoords = new glm::vec2[m->numVertices];
		m->textureCoords[0] = glm::vec2(0.5f, 0.0f);
		m->textureCoords[1] = glm::vec2(1.0f, 1.0f);
		m->textureCoords[2] = glm::vec2(0.0f, 1.0f);

		m->colours = new glm::vec4[m->numVertices];
		m->colours[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m->colours[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		m->colours[2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		m->colours[3] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m->colours[4] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		m->colours[5] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		m->BufferData();
		return m;
	}

	static MeshX* GenerateQuad() {
		MeshX* m = new MeshX();
		m->numVertices = 4;
		m->type = GL_TRIANGLE_STRIP;

		m->vertices = new glm::vec3[m->numVertices];
		m->textureCoords = new glm::vec2[m->numVertices];
		m->colours = new glm::vec4[m->numVertices];

		m->vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
		m->vertices[1] = glm::vec3(-1.0f, 1.0f, 0.0f);
		m->vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);

		m->textureCoords[0] = glm::vec2(0.0f, 1.0f);
		m->textureCoords[1] = glm::vec2(0.0f, 0.0f);
		m->textureCoords[2] = glm::vec2(1.0f, 1.0f);

		for (int i = 0; i < 4; ++i) {
			m->colours[i] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		m->BufferData();
		return m;
	}

	void SetTexture(int tex) { texture = tex; }
	unsigned int GetTexture() { return texture; }


	void BufferData() {
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_BUFFER);

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

		if (normals) {
			glGenBuffers(1, &VBO[NORMAL_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
			glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(NORMAL_BUFFER);
		}

		glBindVertexArray(0);
	}

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
		else { // It ¡¯s just a list of triangles , so generate face normals
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
	}

	unsigned int VAO;
	unsigned int VBO[MAX_BUFFER];
	unsigned int numVertices;
	unsigned int type;

	glm::vec3* vertices;
	glm::vec4* colours;

	glm::vec2* textureCoords;
	unsigned int texture;

	unsigned int numIndices;
	unsigned int* indices;

	glm::vec3* normals;
};