// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "CameraData.h"

Matrix4f CameraData::GetViewMatrix()
{
	return glm::translate(Matrix4f(), transform.location);
}

Matrix4f CameraData::GetProjectionMatrix(const v2_u32& screenSize)
{
	return glm::perspectiveFov<float>(fov * Math::DEGTORAD, float(screenSize.x), float(screenSize.y), nearZ, farZ);
}
