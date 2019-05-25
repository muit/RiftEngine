// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CKey : public Component {
	STRUCT(CKey, Component)

	PROP(Name, keyId, DetailsEdit)
	Name keyId;

	PROP(bool, bPickedUp, DetailsView)
	bool bPickedUp;
};
