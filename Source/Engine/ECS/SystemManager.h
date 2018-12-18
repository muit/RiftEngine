// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "System.h"


class SystemsManager : public Object {
	CLASS(SystemsManager, Object)

	eastl::vector<GlobalPtr<System>> systems;


public:

	void BeginPlay() {
		IterateSystems([](Ptr<System> system) {
			system->BeginPlay();
		});
	}

	void Tick(float deltaTime) {
		IterateSystems([deltaTime](Ptr<System> system) {
			system->Tick(deltaTime);
		});
	}

	void EndPlay() {
		IterateSystems([](Ptr<System> system) {
			system->BeginPlay();
		});
	}

	void IterateSystems(eastl::function<void(Ptr<System>)> callback) const {
		for (const GlobalPtr<System>& system : systems)
		{
			if (system)
			{
				callback(system);
			}
		}
	}
};
