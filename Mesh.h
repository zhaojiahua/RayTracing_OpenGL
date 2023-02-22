#pragma once
#include <glad/glad.h>

//给定Mesh的顶点数据
class Mesh
{
public:
	Mesh();
	~Mesh();
	//传入顶点数据(依次包含顶点位置,法线,UV信息),顶点的个数和顶点序列号数据,顶点序列号个数
	Mesh(GLfloat* vertices, GLuint sizeofvertices, GLuint* indices, GLuint sizeofindices);

	//指向顶点数据的首位置指针
	GLfloat* mVertices;
	//指向序列号数组的首位置指针
	GLuint* mIndices;
	GLuint mVAO, mVBO, mEBO, mSizeofVertices, mSizeofIndices;
};

