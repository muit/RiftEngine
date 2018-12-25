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

		// Test entity
		entityManager->CreateEntity(TX("MyEntity"));
		entityManager->CreateEntity(TX("MyOtherEntity"));
	}

	void Tick(float deltaTime) {
		ZoneScopedN("World Tick");
		scene->Tick(deltaTime);
		systemManager->Tick(deltaTime);
	}

	void Render() {
		systemManager->Render();
	}

	void EndPlay() {
		systemManager->EndPlay();
	}

	Ptr<Scene> GetScene() { return scene; }


	Ptr<SystemManager> GetSystemManager() { return systemManager; }
	Ptr<EntityManager> GetEntityManager() { return entityManager; }
};
