// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Math/Transform.h"


struct CameraData {

public:

	float fov;
	float nearZ;
	float farZ;
	float aspect;
	Transform transform;


	CameraData()
		: fov{ 60.f }
		, nearZ{ 0.1f }
		, farZ{ 1000.f }
		, aspect{ 1.f }
		, transform{}
	{}


	Matrix4f GetPerspectiveMatrix() {
		const float depth_length = farZ - nearZ;
		const float tanHalfFov = std::tan(fov * 0.5f * Math::DEGTORAD);

		Matrix4f matrix{};
		matrix(0, 0) = 1.f / (aspect * tanHalfFov);
		matrix(1, 1) = 1.f / tanHalfFov;
		matrix(2, 2) = -(nearZ + farZ)       / depth_length;
		matrix(2, 3) = -(2.f * nearZ * farZ) / depth_length;
		matrix(3, 2) = -1.f;
		return matrix;
	}
};
