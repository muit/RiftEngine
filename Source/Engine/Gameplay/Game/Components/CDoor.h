// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CDoor : public Component {
	STRUCT(CDoor, Component)

	PROP(Name, keyId, DetailsEdit)
	Name keyId;

	PROP(bool, bOpened, DetailsView)
	bool bOpened;
};
