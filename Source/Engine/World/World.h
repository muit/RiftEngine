// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Assets/Scene.h"

#include "ECS/ECSManager.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Rendering/Frame.h"
#include "Core/Rendering/RenderCommand.h"
#include "Core/Assets/AssetPtr.h"


class World : public Object {
	CLASS(World, Object)


	GlobalPtr<AssetManager> assetManager;

	TAssetPtr<Scene> scene;

	GlobalPtr<ECSManager> ecsManager;

	Frame* currentFrame;


public:

	void Start() {
		assetManager = Create<AssetManager>(GetSelf());

		scene = { "scene.meta" };
		scene.LoadOrCreate();

		ecsManager = Create<ECSManager>(GetSelf());
		ecsManager->BeginPlay();

		// Test entities
		ecsManager->CreateEntity(TX("MyEntity"));
		ecsManager->CreateEntity(TX("MyOtherEntity"));
	}

	void Tick(Frame& frame, float deltaTime) {
		ZoneScopedNC("World", 0x94d145);
		currentFrame = &frame;

		scene->Tick(deltaTime);
		ecsManager->Tick(deltaTime);
	}

	void EndPlay() {
		ecsManager->EndPlay();
	}

	TAssetPtr<Scene> GetActiveScene() const { return scene; }
	Ptr<AssetManager> GetAssetManager()   const { return assetManager; }
	Ptr<ECSManager> GetECS() const { return ecsManager; }

	FORCEINLINE bool IsEditor() {
#if WITH_EDITOR
		return true;
#else
		return false;
#endif
	}



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
