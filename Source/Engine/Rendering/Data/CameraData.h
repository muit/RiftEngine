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
		, farZ{ 15.f }
		, transform{}
	{}


	Matrix4f GetPerspectiveMatrix(const v2_u32& screenSize) {
		const float depth_length = 1.f / (farZ - nearZ);
		const float scale = 1.f / std::tan(fov * 0.5f * Math::DEGTORAD);

		const float aspect = float(screenSize.x()) / float(screenSize.y());

		Matrix4f matrix{ Matrix4f::Zero() };
		matrix(0, 0) = scale / aspect;
		matrix(1, 1) = scale;
		matrix(2, 2) = -(nearZ + farZ) * depth_length;
		matrix(2, 3) = -2.f * nearZ * farZ * depth_length;
		matrix(3, 2) = -1.f;
		return matrix;
	}
};
