#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//射线类,用于发射模拟光线
class Ray
{
public:
	glm::vec3 mOrgPosition, mDirection;
public:
	Ray() {};
	//初始化需要给定发射位置和发射方向
	Ray(const glm::vec3& origin, const glm::vec3& direction) :mOrgPosition(origin), mDirection(direction) {};

	//根据射线起始点位置和差值t 返回射线的打到的位置
	glm::vec3 At(GLfloat t)const {
		return mOrgPosition + t * mDirection;
	}
};

