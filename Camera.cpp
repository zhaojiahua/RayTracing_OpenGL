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
	mViewMatrix = glm::lookAt(mPosition, mPosition + mDirection, mUp);
}
