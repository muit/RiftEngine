// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <entt/entity/registry.hpp>

#include "Gameplay/Components/CEntity.h"
#include "System.h"


using EntityId = u32;
class Archive;


class ECSManager : public Object {
	CLASS(ECSManager, Object)


	entt::Registry<EntityId> registry;

	TArray<GlobalPtr<System>> systems;

public:

	/**************************************************************
	 * Begin ENTITIES
	 */

	EntityId CreateEntity(Name entityName)
	{
		EntityId ent = registry.create();
		registry.assign<CEntity>(ent, entityName);
		return ent;
	}

	void DestroyEntity(EntityId entity)
	{
		registry.destroy(entity);
	}

	bool IsValid(EntityId entity) const
	{
		return registry.valid(entity);
	}


	/**************************************************************
	 * Begin SERIALIZATION
	 */

	virtual bool Serialize(Archive& ar) override;

	void SerializeEntity(Archive& ar, const EntityId& entity);

private:

	template<typename CompType>
	void SerializeComponent(Archive& ar, const EntityId& entity) {
		static_assert(eastl::is_convertible< CompType, Component >::value, "Type is not a Component!");
		if (registry.has<CompType>(entity))
		{
			ar.BeginObject(CompType::StaticStruct()->GetSName());
			{
				CompType& comp = registry.get<CompType>(entity);
				comp.SerializeReflection(ar);
			}
			ar.EndObject();
		}
	}


public:

	/**************************************************************
	 * Begin SYSTEMS
	 */

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

	template<typename... Component>
	auto View() {
		return registry.view<Component...>();
	};


private:

	void RegistrySystems();

	template <typename T>
	void RegistrySystem() {
		systems.Add(Create<T>(GetSelf()));
	}
};
