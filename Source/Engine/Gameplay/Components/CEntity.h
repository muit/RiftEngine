// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CEntity : public Component {
	STRUCT(CEntity)

public:

	CEntity() : id{Guid::New()} {}

	PROP(Guid, id, DetailsEdit | SaveGame)
	Guid id;

	PROP(Name, name, DetailsEdit | SaveGame)
	Name name;
};
