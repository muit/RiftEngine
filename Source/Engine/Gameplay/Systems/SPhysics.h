// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Gameplay/Components/CTransform.h"
#include "Physics/Components/CBoxCollider2D.h"


using BoxView = entt::View<EntityId, CTransform, CBoxCollider2D>;


class SPhysics : public System {
	CLASS(SPhysics, System)

protected:

	virtual void Tick(float deltaTime) override;

	void UploadDataToPhysics(BoxView& view);
	void ApplyPhysicsData(BoxView& view);
};
