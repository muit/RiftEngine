// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Reflection/Reflection.h"

class Object;

class BaseObject : public std::enable_shared_from_this<BaseObject> {
	ORPHAN_CLASS(BaseObject)

public:

	BaseObject() = default;

	virtual void BeforeDestroy() {}

	virtual ~BaseObject() {}
};
