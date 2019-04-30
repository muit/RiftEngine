// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "EngineTypes.h"
#include "ECS/EntityId.h"
#include "ECS/Component.h"


class CEntity : public Component {
	STRUCT(CEntity, Component)

public:

	PROP(Guid, id, Transient)
	Guid id;

	PROP(Name, name, DetailsEdit | SaveGame)
	Name name;

	PROP(bool, bTransient)
	bool bTransient;

	/** Parent entity */
	PROP(EntityId, parent)
	EntityId parent;

private:

	PROP(bool, bPendingKill)
	bool bPendingKill;


public:

	CEntity()
		: id{ Guid::New() }
		, name{}
		, bTransient{ false }
		, parent{}
	{}

	CEntity(Name name, bool bTransient)
		: id{ Guid::New() }
		, name{ name }
		, bTransient{bTransient}
		, parent{}
	{}

	void MarkPendingKill() { bPendingKill = true; }
	FORCEINLINE bool IsPendingKill() const { return bPendingKill; }
};
