#include "Shader.h"

Shader::Shader()
{
	ID = glCreateProgram();
}
Shader::Shader(string vertexfile, string fragmentfile)
{
	ID = glCreateProgram();
	string vertexsource, fragmentsource;
	ifstream vertexfileH, fragmentfileH;

	vertexfileH.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentfileH.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		//打开文件读取源码
		vertexfileH.open(vertexfile); fragmentfileH.open(fragmentfile);
		stringstream vertexstrstream, fragmentstrstream;
		//读入数据
		vertexstrstream << vertexfileH.rdbuf(); fragmentstrstream << fragmentfileH.rdbuf();
		//关闭文件
		vertexfileH.close(); fragmentfileH.close();
		//保存数据到string
		vertexsource = vertexstrstream.str(); fragmentsource = fragmentstrstream.str();
	}
	catch (ifstream::failure err)
	{
		cout << "Error: shader file not succesefully read!" << endl;
	}
	//string转成char* 类型
	const char* vertexshadercode = vertexsource.c_str();
	const char* fragmentshadercode = fragmentsource.c_str();

	//开始创建着色器
	GLuint vertexShader, fragmentShader;
	GLint success;
	char infoLog[512];
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexshadercode, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		cout << "Error:vertex shader compile failed" << infoLog << endl;
	}
	fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentshadercode, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		cout << "Error:fragment shader compile failed" << infoLog << endl;
	}
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		cout << "Error:shader program link failed" << infoLog << endl;
	}
	//程序链接完成后删除着色器
	glDeleteShader(vertexShader); glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetFloatUniform(string inname, const GLfloat& inv)
{
	GLint tempLocal = glGetUniformLocation(ID, inname.c_str());
	glUniform1f(tempLocal, inv);
}

void Shader::SetVec2Uniform(string inname, const glm::vec2& inv)
{
	GLint tempLocal = glGetUniformLocation(ID, inname.c_str());
	glUniform2f(tempLocal, inv[0], inv[1]);
}

void Shader::SetMat4Uniform(string inname, const glm::mat4& inmatrix)
{
	GLint tempLocal=glGetUniformLocation(ID, inname.c_str());
	glUniformMatrix4fv(tempLocal, 1, GL_FALSE, glm::value_ptr(inmatrix));
}
