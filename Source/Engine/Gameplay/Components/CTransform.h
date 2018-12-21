// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CTransform : public Component {
	STRUCT(CTransform)

public:

	PROP(Name, name, Transient | SaveGame)
	Name name;
};
