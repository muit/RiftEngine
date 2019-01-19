// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Vector.h"
#include "Math.h"


struct Transform {

	v3 location;
	Quat rotation;
	v3 scale;


	Transform() : location{ v3::Zero() }, rotation{ Quat::Identity() }, scale{ v3::Ones() } {};
	Transform(v3 location) : location{ location }, rotation{ Quat::Identity() }, scale{ v3::Ones() } {};


	FORCEINLINE v3 GetRotationRadians() {
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

#if WITH_EDITOR
	static class Class* GetDetailsWidgetClass();
#endif
};

DEFINE_CLASS_TRAITS(Transform,
	HasCustomSerialize = true,
	HasDetailsWidget = true
);
