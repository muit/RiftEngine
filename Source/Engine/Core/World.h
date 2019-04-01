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


public:

	void Start()
	{
		assetManager = Create<AssetManager>(Self());


		ecsManager = Create<ECSManager>(Self());

		scene = { "scene.meta" };
		scene.LoadOrCreate();

		ecsManager->BeginPlay();

		// Test entities. Move to Scene
		/*ecsManager->CreateEntity(TX("MyEntity"));
		EntityId b = ecsManager->CreateEntity(TX("MyOtherEntity"));
		Transform& t = ecsManager->Assign<CTransform>(b).transform;
		t.location = { 0, 0, -10 };
		t.SetRotationDegrees({ 0,0,0 });
		t.scale = { 0.2f, 0.2f, 0.2f };
		ecsManager->Assign<CMesh>(b, TAssetPtr<Model>{"Terrain.obj.meta"});*/
	}

	void Tick(float deltaTime) {
		ZoneScopedNC("World", 0x459bd1);

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
};
