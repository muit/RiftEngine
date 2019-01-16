// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/EntityManager.h"
#include "Core/Math/Transform.h"


class CTransform : public Component {
	POD(CTransform)

private:

	EntityId parent;

	PROP(Transform, transform, DetailsEdit);
	Transform transform;
};
