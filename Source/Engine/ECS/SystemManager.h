// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "System.h"

#include "Gameplay/Systems/SRenderCamera.h"
#include "Gameplay/Systems/SRenderMesh.h"


class SystemManager : public Object {
	CLASS(SystemManager, Object)

	eastl::vector<GlobalPtr<System>> systems;


	void RegistrySystems() {
		// #TODO: Externalize system registry
		RegistrySystem<SRenderMesh>();
		RegistrySystem<SRenderCamera>();
	}

public:

	void BeginPlay() {

		RegistrySystems();
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
			system->EndPlay();
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

private:

	template <typename T>
	void RegistrySystem() {
		systems.push_back(Create<T>(GetSelf()));
	}
};
