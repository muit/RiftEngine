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


	FORCEINLINE Rotator GetRotation() {
		return rotation.ToRotator();
	}

	//v3 GetWorldLocation(const Transform& parent) { return location; }
	//v3 GetWorldRotation() { return value.rotation(); }
	//v3 GetWorldScale()    { return value.rotation(); }

	void SetRotation(const Rotator& angles) {
		rotation = Quat::FromRotator(angles);
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

	v3 GetForward() const {
		return rotation.GetForward();
	}


#if WITH_EDITOR
	static class Class* GetDetailsWidgetClass();
#endif
};

DEFINE_CLASS_TRAITS(Transform,
	HasCustomSerialize = true,
	HasDetailsWidget = true
);
