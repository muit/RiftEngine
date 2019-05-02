// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Assets/Scene.h"

#include "Core/Assets/AssetManager.h"
#include "Physics/PhysicsManager.h"
#include "ECS/ECSManager.h"
#include "Rendering/Frame.h"
#include "Rendering/Commands/RenderCommand.h"
#include "Core/Assets/AssetPtr.h"

#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Components/CTransform.h"
#include "../Assets/Material.h"


class World : public Object {
	CLASS(World, Object)

	TAssetPtr<Scene> scene;

	GlobalPtr<PhysicsManager> physics;
	GlobalPtr<ECSManager> ecs;


public:

	void Initialize();

	void Tick(float deltaTime);

	void EndPlay() { ecs->EndPlay(); }

	TAssetPtr<Scene> GetActiveScene() const { return scene; }
	Ptr<PhysicsManager> GetPhysics()   const { return physics; }
	Ptr<ECSManager> GetECS() const { return ecs; }

	FORCEINLINE bool IsEditor() {
#if WITH_EDITOR
		return true;
#else
		return false;
#endif
	}
};
