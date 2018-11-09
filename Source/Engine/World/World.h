// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Object.h"
#include "Scene.h"


class World : public Object {
	CLASS(World, Object)

	GlobalPtr<Scene> scene;

public:

	void Start() {
		scene = Create<Scene>(GetSelf());
	}

	void Tick(float deltaTime) {
		ZoneScopedN("World Tick");
		scene->Tick(deltaTime);
	}

	Ptr<Scene> GetScene() { return scene; }
};
