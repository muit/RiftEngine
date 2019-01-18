// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Vector.h"


struct Transform {

	v3 location;
	Quat rotation;
	v3 scale;


	Transform() : location{ v3::Zero() }, rotation{ Quat::Identity() }, scale{ v3::Ones() } {};
	Transform(v3 location) : location{ location }, rotation{ Quat::Identity() }, scale{ v3::Ones() } {};


	v3 GetLocation() { return location; }
	Quat GetRotation() { return rotation; }
	v3 GetScale() { return scale; }

	//v3 GetWorldLocation(const Transform& parent) { return location; }
	//v3 GetWorldRotation() { return value.rotation(); }
	//v3 GetWorldScale()    { return value.rotation(); }

	//void SetLocation(v3 location) { value.translate(location); }
	//void SetRotation(v3 rotation) { value.rotate(rotation); }

	bool Serialize(class Archive& ar, const char* name);

#if WITH_EDITOR
	static class Class* GetDetailsWidgetClass();
#endif
};

DEFINE_CLASS_TRAITS(Transform,
	HasCustomSerialize = true,
	HasDetailsWidget = true
);
