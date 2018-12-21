// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"


class Scene : public Object {
	CLASS(Scene, Object)

public:

	PROP(String, displayName)
	String displayName;

	PROP(Name, someName)
	Name someName;

	PROP(int32, aNumber)
	int32 aNumber;

	PROP(uint8, anotherNumber, Transient)
	uint8 anotherNumber;

	void Tick(float /*deltaTime*/) {
	}
};
