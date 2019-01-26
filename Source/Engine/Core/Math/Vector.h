// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/TypeTraits.h"
#include "Eigen/Core.h"
#include "Eigen/Geometry.h"


using v2 = Eigen::Matrix<float, 2, 1>;
using v2_u32 = Eigen::Matrix<u32, 2, 1>;

using v3 = Eigen::Matrix<float, 3, 1>;


/** Non reflected vectors */

using v4 = Eigen::Matrix<float, 4, 1>;

using v2_u8 = Eigen::Matrix<u8, 2, 1>;
using v3_u8 = Eigen::Matrix<u8, 3, 1>;
using v4_u8 = Eigen::Matrix<u8, 4, 1>;

using v2_i32 = Eigen::Matrix<i32, 2, 1>;
using v3_i32 = Eigen::Matrix<i32, 3, 1>;
using v4_i32 = Eigen::Matrix<i32, 4, 1>;

using v3_u32 = Eigen::Matrix<u32, 3, 1>;
using v4_u32 = Eigen::Matrix<u32, 4, 1>;

using Quat = Eigen::Quaternion<float, Eigen::DontAlign>;

using Matrix4f = Eigen::Matrix4f;


template<typename Type, u32 Dimensions>
struct box {
	using VectorType = Eigen::Matrix<Type, Dimensions, 1>;

	VectorType min;
	VectorType max;

	box() = default;
	constexpr box(VectorType min, VectorType max) : min{min}, max{max} {}

	inline void ExtendPoint(const VectorType& point) {
		for (u32 i = 0; i < Dimensions; ++i)
		{
			if (point[i] < min[i]) {
				min[i] = point[i];
			}

			if (point[i] > max[i]) {
				max[i] = point[i];
			}
		}
	}

	// Limit this bounds by another
	inline void Cut(const box& other) {
		for (u32 i = 0; i < Dimensions; ++i)
		{
			min[i] = Math::Clamp(min[i], other.min[i], other.max[i]);
			max[i] = Math::Clamp(max[i], other.min[i], other.max[i]);
		}
	}


	// Limit this bounds by another
	inline void Extend(const box& other) {
		for (u32 i = 0; i < Dimensions; ++i)
		{
			if (other.min[i] < min[i]) {
				min[i] = other.min[i];
			}
			if (other.max[i] > max[i]) {
				max[i] = other.max[i];
			}
		}
	}
};

using box2 = box<float, 2>;
using box3 = box<float, 3>;
using box2_i32 = box<i32, 2>;
using box3_i32 = box<i32, 3>;
