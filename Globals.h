#pragma once
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Render.h"
#include "Ray.h"
#include "Camera.h"

#include <random>
#include <functional>

#include <json/json.h>
#include <opencv2/opencv.hpp>

#define PI 3.1415926535898

using namespace std;

//全局变量
// //画布尺寸
GLuint screenWidth = 1800, screenHeigh = 1200;
//画面尺寸
GLuint viewWidht = 1800, viewHeight = 1200;

Camera* camera1 = new Camera(0.1f, 100.0f, (GLfloat)(static_cast<GLfloat>(screenWidth) / static_cast<GLfloat>(screenHeigh)), 51.5f);

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

//从磁盘文件读取数据(TXT格式的文件)
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

//从json文件提取数据(位置 pscale mat)(houdini文件直接保存成json格式)
vector<vector<float>> ReadJson(string infile) {
	vector<vector<float>>tempResult;
	ifstream ifs(infile);
	Json::Reader rd;
	Json::Value root;
	rd.parse(ifs, root);
	if (root.isArray()) {
		//读取点个数
		int pointcount = root[5].asInt();
		//读取点位置信息(sub6Root_position里面提取了包含N个点的三维坐标)
		Json::Value sub6Root_position = root[15][1][0][1][7][5];
		//读取点mat信息(sub7Root_mat里面提取了包含N个点mat信息)
		Json::Value sub7Root_mat = root[15][1][1][1][7][5][0];
		//读取点pscale信息(sub7Root_pscale里面提取了包含N个点pscale信息)
		Json::Value sub7Root_pscale = root[15][1][2][1][7][5][0];
		vector<float>tempVector;
		for (int i = 0; i < pointcount; i++) {
			tempVector.clear();
			//位置
			tempVector.push_back(sub6Root_position[i][0].asFloat());
			tempVector.push_back(sub6Root_position[i][1].asFloat());
			tempVector.push_back(sub6Root_position[i][2].asFloat());
			//pscale
			tempVector.push_back(sub7Root_pscale[i].asFloat());
			//mat
			tempVector.push_back(sub7Root_mat[i].asFloat());
			tempResult.push_back(tempVector);
		}
	}
	ifs.close();
	return tempResult;
}

//输出图片(将内存数据存储到硬盘)
void DumpImage(const char* imgData, uint32_t frameid, uint32_t width, uint32_t height, uint32_t bytesPerPixel) {
	string filename = "outimgs/test_" + to_string(frameid) + ".jpg";
	int iSize = width * height * bytesPerPixel;
	ofstream outfp(filename.c_str(), ios::out | ios::binary);
	if (!outfp.is_open()) {
		cout << filename + " open failed" << endl;
		return;
	}
	outfp.write(imgData, sizeof(char) * iSize);
	outfp.close();
}

//cv test
void CVTest(string filename) {
	cv::Mat img = cv::imread(filename);
	cv::namedWindow("cvtest");
	cv::imshow("cvtest", img);
	cv::waitKey(8000);
}