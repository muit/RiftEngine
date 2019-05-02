// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "CameraData.h"
#include <glm/gtc/matrix_transform.hpp>

Matrix4f CameraData::GetViewMatrix()
{
	// Translate -> Rotate
	return Matrix4f(glm::mat4_cast(transform.rotation) * glm::translate(transform.location)).Inverse();
}

Matrix4f CameraData::GetProjectionMatrix(const v2_u32& screenSize)
{
	return glm::perspective(fov * Math::DEGTORAD, float(screenSize.x) / float(screenSize.y), nearZ, farZ);
}
