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

	PROP(bool, bTransient, Transient)
	bool bTransient;


	CEntity() : id{ Guid::New() }, name{}, bTransient{false} {}
	CEntity(Name name, bool bTransient)
		: id{ Guid::New() }
		, name{ name }
		, bTransient{bTransient}
	{}
};
