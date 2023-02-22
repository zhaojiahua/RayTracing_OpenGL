#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	delete(mVertices);
	delete(mIndices);
}

Mesh::Mesh(GLfloat* vertices,GLuint sizeofvertices,GLuint* indices,GLuint sizeofindices)
{

	mSizeofIndices = sizeofindices;
	mSizeofVertices = sizeofvertices;

	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glGenVertexArrays(1, &mVAO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeofvertices, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofindices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);//设置VAO的0号位置属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));//设置VAO的1号法线属性
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float)));//设置VAO的2号UV属性
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
