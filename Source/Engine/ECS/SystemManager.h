// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Object.h"
#include "System.h"


class SystemsManager : public Object {
	CLASS(SystemsManager, Object)

	eastl::vector<GlobalPtr<System>> systems;


public:

	virtual void Tick(float deltaTime) {
		for (const GlobalPtr<System>& system : systems)
		{
			if (system)
			{
				system->Tick(deltaTime);
			}
		}
	}
};
