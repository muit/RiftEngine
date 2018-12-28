// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/EntityManager.h"


class CTransform : public Component {
	STRUCT(CTransform)

public:

	// #TODO: Implement Vector3 & Quaternion types

	// In the future hierarchy will be implemented. Similar to unity in this sense. Entities will contain entities
	//EntityId parent;
};
