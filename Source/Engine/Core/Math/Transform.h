// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Vector.h"
#include "Math.h"
#include "EASTL/internal/copy_help.h"


struct Transform {

	using Matrix = Eigen::Transform<float, 3, Eigen::Affine>;

	v3 location;
	Quat rotation;
	v3 scale;


	Transform() : location{ v3::Zero() }, rotation{ Quat::Identity() }, scale{ v3::Ones() } {};
	Transform(v3 location) : location{ location }, rotation{ Quat::Identity() }, scale{ v3::Ones() } {};


	FORCEINLINE v3 GetRotationRadians() {
		// #REFACTOR: Angle operations should go else where
		return rotation.toRotationMatrix().eulerAngles(0, 1, 2) + v3::Zero(); // + 0 removed -0 errors
	}
	FORCEINLINE v3 GetRotationDegrees() {
		return GetRotationRadians() * Math::RADTODEG;
	}

	//v3 GetWorldLocation(const Transform& parent) { return location; }
	//v3 GetWorldRotation() { return value.rotation(); }
	//v3 GetWorldScale()    { return value.rotation(); }

	void SetRotationRadians(const v3& angles) {
		rotation = Eigen::AngleAxisf(angles.x(), v3::UnitX())
				 * Eigen::AngleAxisf(angles.y(), v3::UnitY())
				 * Eigen::AngleAxisf(angles.z(), v3::UnitZ());
	}

	void SetRotationDegrees(v3 angles) {
		angles /= Math::RADTODEG;
		SetRotationRadians(angles);
	}

	bool Serialize(class Archive& ar, const char* name);


	Matrix ToWorldMatrix() const {
		return Eigen::Translation3f(location) * rotation.normalized().toRotationMatrix() * Scaling(scale);
	}

	Matrix ToLocalMatrix() const {
		return ToWorldMatrix().inverse();
	}

	v3 LocationToWorld(const v3& inLocation) const {
		return ToWorldMatrix() * inLocation;
	}

	v3 LocationToLocal(const v3& inLocation) const {
		return ToWorldMatrix().inverse() * inLocation;
	}

	v3 DirectionToWorld(const v3& direction) const {
		return ToWorldMatrix().linear() * direction;
	}

	v3 DirectionToLocal(const v3& direction) const {
		return ToWorldMatrix().inverse().linear() * direction;
	}


#if WITH_EDITOR
	static class Class* GetDetailsWidgetClass();
#endif
};

DEFINE_CLASS_TRAITS(Transform,
	HasCustomSerialize = true,
	HasDetailsWidget = true
);
