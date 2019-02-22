// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/TypeTraits.h"
#include "Eigen/Core.h"
#include "Eigen/Geometry.h"

#include "Math.h"


using v2 = Eigen::Matrix<float, 2, 1>;
using v2_u32 = Eigen::Matrix<u32, 2, 1>;

class v3 : public Eigen::Matrix<float, 3, 1> {
	using Eigen::Matrix<float, 3, 1>::Matrix;

public:

	static const v3 Forward;
	static const v3 Right;
	static const v3 Up;
};


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

class Rotator : public v3 {
	using v3::v3;

public:

	float Pitch() const { return y(); }
	float Yaw()   const { return z(); }
	float Roll()  const { return x(); }

	float& Pitch() { return y(); }
	float& Yaw()   { return z(); }
	float& Roll()  { return x(); }

	/**
	 * Clamps an angle to the range of [0, 360).
	 *
	 * @param Angle The angle to clamp.
	 * @return The clamped angle.
	 */
	static float ClampAxis(float Angle);

	/**
	 * Clamps an angle to the range of (-180, 180].
	 *
	 * @param Angle The Angle to clamp.
	 * @return The clamped angle.
	 */
	static float NormalizeAxis(float Angle);
};

class Quat : public Eigen::Quaternion<float, Eigen::DontAlign> {
	using Eigen::Quaternion<float, Eigen::DontAlign>::Quaternion;

public:

	Rotator ToRotator() const;

	Rotator ToRotatorRad() const {
		return ToRotator() * Math::DEGTORAD;
	}

	v3 GetForward() const { return *this * v3::Forward; }
	v3 GetRight()   const { return *this * v3::Right;   }
	v3 GetUp()      const { return *this * v3::Up;      }

	static Quat FromRotator(Rotator rotator);

	static Quat FromRotatorRad(Rotator rotator) {
		return FromRotator(rotator * Math::DEGTORAD);
	}

	static Quat LookAt(v3 origin, v3 dest);
};

using Matrix4f = Eigen::Matrix4f;


template<typename Type, u32 Dimensions>
struct Box {
	using VectorType = Eigen::Matrix<Type, Dimensions, 1>;

	VectorType min;
	VectorType max;


	Box() = default;
	constexpr Box(VectorType min, VectorType max) : min{min}, max{max} {}

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
	inline void Cut(const Box& other) {
		for (u32 i = 0; i < Dimensions; ++i)
		{
			min[i] = Math::Clamp(min[i], other.min[i], other.max[i]);
			max[i] = Math::Clamp(max[i], other.min[i], other.max[i]);
		}
	}

	// Extend this bounds by another
	inline void Extend(const Box& other) {
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

	// Limit this bounds by another
	FORCEINLINE bool Contains(const Box& other) {
		for (u32 i = 0; i < Dimensions; ++i)
		{
			if (other.min[i] < min[i] || other.max[i] > max[i]) {
				return false;
			}
		}
		return true;
	}
};

using box2 = Box<float, 2>;
using box3 = Box<float, 3>;
using box2_i32 = Box<i32, 2>;
using box3_i32 = Box<i32, 3>;
