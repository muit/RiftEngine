// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"
#include "Core/Math/Vector.h"


class Scene : public Object {
	CLASS(Scene, Object)

public:

	PROP(String, displayName, DetailsEdit)
	String displayName;

	PROP(Name, someName, DetailsEdit)
	Name someName;

	PROP(int32, aNumber, DetailsEdit)
	int32 aNumber;

	PROP(uint8, anotherNumber, DetailsEdit | Transient)
	uint8 anotherNumber;

	PROP(v3, myVector, DetailsEdit)
	v3 myVector;

	void Tick(float /*deltaTime*/) {
	}
};
