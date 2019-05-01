// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Math/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct CameraData {

public:

	float fov;
	float nearZ;
	float farZ;
	Transform transform;


	CameraData()
		: fov{ 60.f } // Obtained from camera
		, nearZ{ 0.1f }
		, farZ{ 1000.f }
		, transform{}
	{}

	Matrix4f GetViewMatrix();

	Matrix4f GetProjectionMatrix(const v2_u32& screenSize);
};
