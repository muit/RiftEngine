// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Object.h"


class Actor : public Object {
	CLASS(Actor, Object)

public:

	PROP(Name, name)
	Name name;
};
