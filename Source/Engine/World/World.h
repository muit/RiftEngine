// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Scene.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"


class World : public Object {
	CLASS(World, Object)

	GlobalPtr<Scene> scene;

	GlobalPtr<EntityManager> entityManager;
	GlobalPtr<SystemManager> systemManager;

public:

	void Start() {
		scene = Create<Scene>(GetSelf());
		entityManager = Create<EntityManager>(GetSelf());
		systemManager = Create<SystemManager>(GetSelf());

		systemManager->BeginPlay();
	}

	void Tick(float deltaTime) {
		ZoneScopedN("World Tick");
		scene->Tick(deltaTime);
		systemManager->Tick(deltaTime);
	}

	void EndPlay() {
		systemManager->EndPlay();
	}

	Ptr<Scene> GetScene() { return scene; }
};
