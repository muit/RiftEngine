// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Math/Transform.h"


class CRigidbody : public Component {
	STRUCT(CRigidbody, Component)

public:

	PROP(bool, bAffectedByGravity, DetailsEdit);
	bool bAffectedByGravity;

	PROP(u32, mass, DetailsEdit);
	u32 mass;
};
