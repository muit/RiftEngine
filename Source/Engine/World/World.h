// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <iostream>
#include <SDL.h>

#include "EngineTypes.h"
#include "Object.h"
#include "Reflection/Reflection.h"


class World : public Object {
	CLASS(World, Object)

public:

	PROP(Name, name, "Save")
	Name name;
};
