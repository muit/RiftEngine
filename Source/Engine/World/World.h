// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <iostream>
#include <SDL.h>

#include "EngineTypes.h"
#include "Reflection/Reflection.h"


class World {
	CLASS(World)

	PROP(Name, name, "Save")
	Name name;
};
