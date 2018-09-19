// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Object.h"
#include "Gameplay/Actor.h"


class Scene : public Object {
	CLASS(Scene, Object)

public:

	PROP(std::vector<Ptr<Actor>>, actors)
	std::vector<Ptr<Actor>> actors;
};
