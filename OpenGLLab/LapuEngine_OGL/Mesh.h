#pragma once
#include "Common.h"

enum MeshBuffer {
	VERTEX_BUFFER, TEXTURE_BUFFER, COLOUR_BUFFER, INDEX_BUFFER, MAX_BUFFER
};

class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	static Mesh* GenerateTriangle();

	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }

	static Mesh* GenerateQuad();

protected:
	void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;

	Vector2* textureCoords;
	GLuint texture;

	GLuint numIndices;
	unsigned int* indices;

};
