// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Assets/Scene.h"

#include "ECS/ECSManager.h"
#include "Core/Assets/AssetManager.h"
#include "Rendering/Frame.h"
#include "Rendering/RenderCommand.h"
#include "Core/Assets/AssetPtr.h"

#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Components/CTransform.h"


class World : public Object {
	CLASS(World, Object)


	GlobalPtr<AssetManager> assetManager;

	TAssetPtr<Scene> scene;

	GlobalPtr<ECSManager> ecsManager;

	Frame* currentFrame;


public:

	void Start(Frame& frame) {
		currentFrame = &frame;

		assetManager = Create<AssetManager>(GetSelf());

		scene = { "scene.meta" };
		scene.LoadOrCreate();

		ecsManager = Create<ECSManager>(GetSelf());
		ecsManager->BeginPlay();

		// Test entities. Move to Scene
		ecsManager->CreateEntity(TX("MyEntity"));
		EntityId b = ecsManager->CreateEntity(TX("MyOtherEntity"));
		Transform& t = ecsManager->Assign<CTransform>(b).transform;
		t.location = { 0, 0, -10 };
		t.scale = { 0.2f, 0.2f, 0.2f };
		ecsManager->Assign<CMesh>(b);

		currentFrame = nullptr;
	}

	void Tick(Frame& frame, float deltaTime) {
		ZoneScopedNC("World", 0x459bd1);
		currentFrame = &frame;

		scene->Tick(deltaTime);
		ecsManager->Tick(deltaTime);

		currentFrame = nullptr;
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
	Frame& GetFrame() {
		assert(currentFrame && "Frame is only available on start-up or tick");
		return *currentFrame;
	}

	template<typename Command, typename ...Args>
	void QueueRender(Args... args) {
		static_assert(eastl::is_base_of<RenderCommand, Command>::value, "Command type must inherit RenderCommand");

		assert(currentFrame && "Commands can only be scheduled on start-up or tick");

		GetFrame().ScheduleCommand(
			eastl::make_shared<Command>(eastl::forward<Args>(args)...)
		);
	}
};
