// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Math/Transform.h"


class CTransform : public Component {
	STRUCT(CTransform, Component)

public:

	PROP(Transform, transform, DetailsEdit);
	Transform transform;

	Transform&       GetWorldTransform()       { return transform; }
	const Transform& GetWorldTransform() const { return transform; }

	Transform GetRelativeTransform() const { return transform; }
};
