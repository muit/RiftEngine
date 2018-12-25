// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <entt/entity/registry.hpp>

#include "Gameplay/Components/CEntity.h"


using EntityId = uint32;
class Archive;


class EntityManager : public Object {
	CLASS(EntityManager, Object)


	entt::Registry<EntityId> registry;


public:

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

	template<typename... Component>
	auto View() {
		return registry.view<Component...>();
	};

private:

	virtual bool Serialize(Archive& ar) override;

	void SerializeEntity(Archive& ar, const EntityId& entity);
};
