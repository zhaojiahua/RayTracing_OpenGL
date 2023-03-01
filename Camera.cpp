#include "Camera.h"

Camera::Camera()
{}

Camera::Camera(GLfloat near, GLfloat far, GLfloat ratio, GLfloat fov) :mNear(near), mFar(far), mRatio(ratio), mFov(fov)
{
	mPerspectiveMatrix = glm::perspective(fov, ratio, near, far);
	mNearHeight = 2 * near * tan(fov * 0.5);
	mNearWidth = mNearHeight * ratio;
	mViewMatrix = glm::lookAt(mPosition, mPosition + mDirection, mUp);
}

void Camera::Updata()
{
	mViewMatrix = glm::lookAt(mPosition, glm::vec3(0.0f), mUp);
	mPerspectiveMatrix = glm::perspective(mFov, mRatio, mNear, mFar);
}

void Camera::SetCameraDir(GLfloat xoffset, GLfloat yoffset)
{
	mYaw += (xoffset / mScreenWidth * 90.0);
	mPitch += (yoffset / mScreenHeight * 90.0);
	if (mPitch > 90.0)mPitch = 90.0;
	if (mPitch < -90.0)mPitch = -90.0;

	GLfloat x = cos(mPitch) * cos(mYaw);
	GLfloat y = cos(mPitch) * sin(mYaw);
	GLfloat z = sin(mPitch);
	mDirection = glm::vec3(x, y, z);
}
