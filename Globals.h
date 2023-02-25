#pragma once
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Render.h"
#include "Ray.h"
#include "Camera.h"

#include <random>
#include <functional>

#define PI 3.1415926535898

using namespace std;

//全局变量
// //画布尺寸
GLuint screenWidth = 1800, screenHeigh = 1200;
//画面尺寸
GLuint viewWidht = 1800, viewHeight = 1200;

Camera* camera1 = new Camera(0.1f, 100.0f, (GLfloat)(static_cast<GLfloat>(screenWidth) / static_cast<GLfloat>(screenHeigh)), 45.0f);

GLfloat lastX = screenWidth / 2.0, lastY = screenHeigh / 2.0;//定义相机的初始朝向

//全局函数

//按键响应回调函数
void ZJH_KeyCallBack(GLFWwindow* inwind, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(inwind, GL_TRUE);
}
//鼠标滚动响应回调函数
void ZJH_ScrollCallBack(GLFWwindow* inwind, double xoffset,double yoffset) {
	camera1->mFov -= 0.1*yoffset;
	camera1->Updata();
}
//鼠标移动响应回调函数
void ZJH_MouseMoveCallBack(GLFWwindow* inwind, double xpos, double ypos) {
	if (camera1->mFreeView) {
		GLfloat xoffset = (xpos - lastX) * camera1->mSensitive;
		GLfloat yoffset = (ypos - lastY) * camera1->mSensitive;
		lastX = xpos; lastY = ypos;
		camera1->SetCameraDir(xoffset, yoffset);
	}
}
//鼠标点击响应回调函数
void ZJH_MouseButtonCallBack(GLFWwindow* inwind, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		camera1->mFreeView = GL_TRUE;
	}
}

//随机数生成函数(输入最小值和最大值的界定区间)
double Random_double(float inmin,float inmax) {
	static uniform_real_distribution<double> distribution(inmin, inmax);
	static mt19937 generator;
	static function<double()>rand_generator = bind(distribution, generator);
	return rand_generator();
}

//从磁盘文件读取数据
vector<vector<float>> ReadFile(string file,int linecount) {
	ifstream infile;
	infile.open(file, ios::in);
	if (!infile.is_open()) {
		cout << file + "read failed" << endl;
	}
	stringstream strstream;
	istringstream istrsteam;
	vector<float> tempv1;
	vector<vector<float>> tempv2;
	strstream << infile.rdbuf();
	istrsteam.str(strstream.str());
	float tempfloat;
	int i = 0;
	while (istrsteam >> tempfloat) {
		tempv1.push_back(tempfloat);
		i++;
		if (i == linecount) {
			tempv2.push_back(tempv1);
			tempv1.clear();
			i = 0;
		}
	}
	infile.close();
	return tempv2;
}