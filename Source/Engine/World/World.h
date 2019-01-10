// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Scene.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Rendering/Frame.h"
#include "Core/Rendering/RenderCommand.h"


class World : public Object {
	CLASS(World, Object)


	GlobalPtr<AssetManager> assetManager;

	GlobalPtr<Scene> scene;

	GlobalPtr<EntityManager> entityManager;
	GlobalPtr<SystemManager> systemManager;

	Frame* currentFrame;


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

	void Tick(Frame& frame, float deltaTime) {
		ZoneScopedNC("World", 0x94d145);
		currentFrame = &frame;

		scene->Tick(deltaTime);
		systemManager->Tick(deltaTime);
	}

	void EndPlay() {
		systemManager->EndPlay();
	}

	Ptr<Scene> GetScene() const { return scene; }
	Ptr<AssetManager> GetAssetManager()   const { return assetManager; }
	Ptr<SystemManager> GetSystemManager() const { return systemManager; }
	Ptr<EntityManager> GetEntityManager() const { return entityManager; }


	// RENDER COMMANDS
	Frame& GetFrame() { return *currentFrame; }

	template<typename Command, typename ...Args>
	void QueueRender(Args... args) {
		static_assert(eastl::is_base_of<RenderCommand, Command>::value, "Command type must inherit RenderCommand");

		GetFrame().ScheduleCommand(
			eastl::make_shared<Command>(eastl::forward<Args>(args)...)
		);
	}
};
