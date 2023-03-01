#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//摄像机类,需要指定摄像机的近切面(相当于屏幕画面),远切面,宽高比,和视张角
class Camera
{
public:
	Camera();
	Camera(GLfloat near, GLfloat far, GLfloat ratio, GLfloat fov);

	GLfloat mNear, mFar, mRatio, mFov;
	//相机近切面的宽高
	GLfloat mNearWidth, mNearHeight;
	//屏幕空间尺寸也传进来
	GLfloat mScreenWidth, mScreenHeight;
	//相机移动的敏感度
	GLfloat mSensitive = 0.001;
	//相机的pitch和yaw值
	GLfloat mPitch = 0.0, mYaw = 0.0;
	GLboolean mFreeView = GL_FALSE;

	//默认位置在原点
	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	//默认朝向z轴负方向
	glm::vec3 mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	//上方向(固定相机上方向)
	glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//相机的透视矩阵(初始化相机的时候就已经确定)
	glm::mat4 mPerspectiveMatrix = glm::mat4(1.0f);
	//相机的视图矩阵(由相机的位置和方向确定)
	glm::mat4 mViewMatrix = glm::mat4(1.0f);

	//更新相机(随着相机空间位置的变动其视图矩阵是实时变动的,还有运行的时候需要调节相机的某些属性参数,也需要实时更新)
	void Updata();
	//根据鼠标移动更新相机朝向
	void SetCameraDir(GLfloat xoffset, GLfloat yoffset);
};

