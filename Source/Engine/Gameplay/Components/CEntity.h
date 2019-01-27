// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CEntity : public Component {
	STRUCT(CEntity, Component)

public:

	PROP(Guid, id, DetailsEdit | SaveGame)
	Guid id;

	PROP(Name, name, DetailsEdit | SaveGame)
	Name name;


	CEntity() : id{ Guid::New() }, name{} {}
	CEntity(Name name) : id{ Guid::New() }, name{ name } {}
};
