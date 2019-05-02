// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"

#if WITH_EDITOR
#include "UI/Widget.h"
#include "ECS/EntityId.h"
#include "Core/Math/Transform.h"


class Guizmo : public Widget {
	CLASS(Guizmo, Widget)

	EntityId entity = NoEntity;


public:

	virtual void Tick(float deltaTime) override;

	void SetEntity(EntityId inEntity);
};

#endif
