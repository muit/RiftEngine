// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Scene.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "Core/Assets/AssetManager.h"


class World : public Object {
	CLASS(World, Object)


	GlobalPtr<AssetManager> assetManager;

	GlobalPtr<Scene> scene;

	GlobalPtr<EntityManager> entityManager;
	GlobalPtr<SystemManager> systemManager;


public:

	void Start() {
		assetManager = Create<AssetManager>(GetSelf());

		scene = Create<Scene>(GetSelf());
		entityManager = Create<EntityManager>(GetSelf());
		systemManager = Create<SystemManager>(GetSelf());

		systemManager->BeginPlay();

		// Test entities
		entityManager->CreateEntity(TX("MyEntity"));
		entityManager->CreateEntity(TX("MyOtherEntity"));
	}

	void Tick(float deltaTime) {
		ZoneScopedN("World");
		scene->Tick(deltaTime);
		systemManager->Tick(deltaTime);
	}

	void Render() {
		ZoneScopedNC("World", 0x94d145);
		systemManager->Render();
	}

	void EndPlay() {
		systemManager->EndPlay();
	}


	Ptr<Scene> GetScene() const { return scene; }
	Ptr<AssetManager> GetAssetManager()   const { return assetManager; }
	Ptr<SystemManager> GetSystemManager() const { return systemManager; }
	Ptr<EntityManager> GetEntityManager() const { return entityManager; }
};
