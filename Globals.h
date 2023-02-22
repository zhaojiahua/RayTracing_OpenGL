#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Render.h"
#include "Ray.h"
#include "Camera.h"

using namespace std;

//全局变量

//画布尺寸
GLuint screenWidth = 1200, screenHeigh = 800;
//画面尺寸
GLuint viewWidht = 1200, viewHeight = 800;


//全局函数

//按键响应回调函数
void ZJH_KeyCallBack(GLFWwindow* inwind, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(inwind, GL_TRUE);
}