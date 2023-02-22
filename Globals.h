#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Render.h"
#include "Ray.h"
#include "Camera.h"

#include <random>
#include <functional>

using namespace std;

//全局变量

//画布尺寸
GLuint screenWidth = 1800, screenHeigh = 1200;
//画面尺寸
GLuint viewWidht = 1800, viewHeight = 1200;


//全局函数

//按键响应回调函数
void ZJH_KeyCallBack(GLFWwindow* inwind, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(inwind, GL_TRUE);
}

//随机数生成函数
double Random_double() {
	static uniform_real_distribution<double> distribution(0.0, 0.5);
	static mt19937 generator;
	static function<double()>rand_generator = bind(distribution, generator);
	return rand_generator();
}