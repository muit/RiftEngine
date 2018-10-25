// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Object.h"
#include "Gameplay/Actor.h"


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

	std::vector<GlobalPtr<Actor>> actors;

	void Tick(float deltaTime) {
	}
};
