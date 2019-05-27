// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Math/Transform.h"


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

	Matrix4f GetViewMatrixNoLocation();

	Matrix4f GetProjectionMatrix(const v2_u32& screenSize);
};
