#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
class Shader
{
public:
	Shader();
	Shader(string vertexfile, string fragmentfile);

	GLuint ID;

	void Use();
	void SetFloatUniform(string inname, const GLfloat& inv);
	void SetVec2Uniform(string inname, const glm::vec2& inv);
	void SetMat4Uniform(string inname, const glm::mat4& inmatrix);
};

