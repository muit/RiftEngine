// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <iostream>
#include <SDL.h>

#include "EngineTypes.h"
#include "Object.h"
#include "Reflection/Reflection.h"
#include "Scene.h"


class World : public Object {
	CLASS(World, Object)

public:

	GlobalPtr<Object> scene;

	void Start() {
		scene = Object::Create<Scene>(ThisPtr());
	}
};
