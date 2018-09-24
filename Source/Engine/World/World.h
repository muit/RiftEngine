// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Object.h"
#include "Scene.h"


class World : public Object {
	CLASS(World, Object)

	GlobalPtr<Scene> scene;

public:

	void Start() {
		scene = Create<Scene>(ThisPtr());
	}

	void Tick(float deltaTime) {
		scene->Tick(deltaTime);
	}

	Ptr<Scene> GetScene() { return scene; }
};
