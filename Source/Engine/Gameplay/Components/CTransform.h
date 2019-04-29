// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Math/Transform.h"


class CTransform : public Component {
	STRUCT(CTransform, Component)

public:

	PROP(Transform, worldTransform, DetailsEdit);
	Transform worldTransform;

	Transform&       GetWorldTransform()       { return worldTransform; }
	const Transform& GetWorldTransform() const { return worldTransform; }

	Transform GetRelativeTransform() const { return worldTransform; }
};
